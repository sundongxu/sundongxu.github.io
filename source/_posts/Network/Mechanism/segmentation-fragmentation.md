---
title: TCP分段与IP分片的区别与联系
date: 2018-06-01 16:00:00
categories:
- Network
tags:
- Segmentation
- TCP
- Fragmentation
- IP
---

## 全文概要
---
我们在学习TCP/IP协议时都知道，TCP报文段如果很长的话，会在发送时发生**分段(Segmentation)**，在接收时进行重组，同样IP数据报在长度超过一定值时也会发生**分片(Fragmentation)**，在接收端再将分片重组。如果之前你和曾经的我一样，经常混淆这两个概念，甚至一度以为两者表示的是同一个协议栈机制，那么本文就将通过详细介绍分段与分片的区别与联系，力图让你对此有一个更为完整、严谨的理解。
<!--more-->

## 背景
---
首先需要强调的一点是，不管你之前从什么渠道获取了关于分段/分片方面的知识，甚至会觉得两者只是叫法不同但实际意思相同，但在本文中，
> **分段特指发生在使用TCP协议的传输层中的数据切分行为**
> **分片特指发生在使用IPv4协议的网络IP层中的数据切分行为**

这一点在文章标题中已经表达得十分明显。

正如上文所述，分段与分片发生在协议栈的不同层次，但目的都一样：

> 都是为了能够传输上层交付的、数据量超过本层传输能力上限的数据，**不得已**才做的数据切分。

注意到这里我用了修饰词，“不得已” — 也就是说在正常情况尽量避免做数据切分，能不分就不分，而只有在特殊场景下才不得不这么做。

这么形容显然是有原因的，最起码有以下两点解释：
1. 数据切分对于协议栈而言，显然使处理逻辑变得更加复杂了，在发送端需要做切分，甚至在路径中的转发设备中也需要切分(后文会介绍这种情况)，在接收端又要做重组，处理开销明显增大，对设备的处理能力提出更高的要求；
2. 在切分过程过不可避免的要为每个数据分片增加必要的协议首部以完成网络传输，在首部中还需要携带必要的顺序、偏移、是否属于同一块大数据等元信息来帮助组装。
</br>

上述额外的空间开销在没有分片的情况下，显然是不需要的，发一块收一块即可，为了传输该数据包仅为其添加协议栈各层首部各一份，更无须设置用于组装的各种元数据，不论是发送还是接收的处理逻辑都更加简单。所以，能不切分就不切分，毫无疑问是正确的。

到此为止，关于数据切分技术出现的背景，包括其必要性和弊端都应该算是比较清晰了。

回到文章主题，前文有提到分段和分片工作在不同协议层，这其实很容易会造成一些疑惑，这些疑惑也一度伴随着我，比如：
- 分段和分片有没有可能同时发生？为什么可能/不能？
- 如果可能，什么场景下会同时发生？
- 如果不能，什么场景下会发生分段？什么场景下又会发生分片？
- 分段和分片的切分与组装的过程是怎么样的？

## 概念
---
要回答以上问题，首先要介绍两个概念：
### 最大传输单元(Maximum Transmission Unit)
---
最大传输单元(Maximum Transmission Unit)，即**MTU**，为数据链路层的最大载荷上限(即IP数据报最大长度)，每段链路的MTU可能都不相同，一条端到端路径的MTU由这条路径上MTU最小的那段链路的MTU决定。

MTU是链路层中的网络对数据帧的一个限制，以以太网为例，MTU通常为1500字节，采用巨帧(Jumbo Frame)时可以达到9000字节。所谓的MTU，是二层协议的一个限制，对不同的二层协议可能有不同的值，只有二层协议为以太网(**Ethernet**)时，MTU一般才取1500字节，注意它不是物理链路介质的限制，只有工作在二层的设备才需要指定MTU的值，如网卡、转发设备端口(统称为**网络接口**)等，通过同一段线缆直连的通信端口或网卡，其MTU值一定相同。

一个IP数据报在以太网中传输，如果它的长度大于当前链路MTU值，就要进行**分片传输(这里指IP层分片)**，使得每片数据报的长度都不超过MTU。分片传输的IP数据报不一定按序到达，但IP首部中的信息能让这些数据报片按序组装。IP数据报的分片与重组是在网络IP层完成的。

### 最大报文段长度(Maximum Segment Size)
---
最大报文段长度(Maximum Segment Size)，即**MSS**，为TCP传输层的最大载荷上限(即应用层数据最大长度)，TCP三次握手期间通过TCP首部选项中的MSS字段通知对端，通常一条TCP连接的MSS取通信双方较小的那一个MSS值，与MTU的换算关系为：
<center>**MTU = MSS + TCP首部长度 + IP首部长度**</center></br>

故在以太网中(网络层以IPv4为例)：
<center>**MSS = 以太网MTU - TCP首部长度 - IPv4首部长度 = 1500 - 20 - 20 = 1460字节**</center></br>


未指定MSS时默认值为536字节，这是因为在Internet中标准的MTU值为576字节，576字节MTU = TCP首部长度20字节 + IPv4首部长度20字节 + 536字节MSS。

一个应用程序如果要发送超过MSS大小的数据，就要进行**分段传输(这里指TCP分段)**，使得每个报文段长度都不超过MSS。分片传输的TCP报文段不一定按序到达，但实现可靠传输的TCP协议中有处理乱序的机制，即利用报文段序列号在接收缓冲区进行数据重排以实现重组。TCP分段的重组是在TCP传输层完成的。

## 区别
---
有了前文的知识准备，不难得出结论：
> **TCP分段的原因是因为TCP报文段大小受MSS限制**
> **IP分片的原因则是因为IP数据报大小受MTU限制**

由于一直有 $MSS \leq MTU$ 成立，很明显，分段后的每一个TCP报文段再加上IP首部后的长度构造成的IP数据报长度都不可能超过MTU，因此也就不需要在网络层进行IP分片了。

相比于为实现面向连接、提供可靠传输服务而具备各种复杂机制的TCP，UDP是简单至极的传输协议，就是在IP协议基础上增加了些无关痛痒的特性以实现端到端的数据传输服务，因此UDP提供的是类似IP的“尽力而为”的传输服务，是不可靠的。而由于UDP协议并不会自行分段，故MSS的限制对其没有作用，因此最终的IP数据报的长度超过了MTU时，网络层会负责执行IP分片。同样，(没有分段功能的)ICMP数据在网络层中同样会出现IP分片的情况。

简而言之：
> UDP不会分段，就由IP来分片。TCP会分段，当然就不用IP来分了！

另外一个值得注意的是，在分片的数据中，传输层的首部只会出现在第一个分片中，IP数据报分片后，只有第一片带有传输层首部(UDP或ICMP等)，后续分片只有IP首部和应用数据，到了目的地后根据IP首部中的信息在网络层进行重组，这一步骤对上层透明，即传输层根本不知道IP层发生了分片与重组。而TCP报文段的每个分段中都有TCP首部，到了目的地后根据TCP首部的信息在传输层进行重组。

TCP分段仅发生在发送端，这是因为在传输过程中，TCP分段是先被封装成IP数据报，再封装在以太网帧中被链路所传输的，并且在端到端路径上通常不会有工作在三层以上，即传输层的设备，故TCP分段不会发生在传输路径中间的某个设备中，在发送端TCP传输层分段后，在接收端TCP传输层重组。

IP分片不仅会发生在在使用UDP、ICMP等没有分段功能的传输层协议的数据发送方，更还会发生在传输途中，甚至有可能都会发生，这是因为原本的大数据报被分片后很可能会经过不同MTU大小的链路，一旦链路MTU大于当前IP分片大小，则需要在当前转发设备(如路由器)中再次分片，但是各个分片只有到达目的地后才会在其网络层重组，而不是像其他网络协议，在下一跳就要进行重组。

所以有一点需要特别强调：
> 发送端进行TCP分段后就一定不会在IP层进行分片，因为MSS本身就是基于MTU推导而来，TCP层分段满足了MSS限制，也就满足了MTU的物理限制。但在TCP分段发生后仍然可能发生IP分片，这是因为TCP分段仅满足了通信两端的MTU要求，传输路径上如经过MTU值比该MTU值更小的链路，那么在转发分片到该条链路的设备中仍会以更小的MTU值作为依据再次分片。当然如果两个通信主机直连，那么TCP连接协商得到的MTU值(两者网卡MTU较小值)就是端到端的路径MTU值，故发送端只要做了TCP分段，则在整个通信过程中一定不会发生IP分片。

特别地，对中途发生分片的数据报而言，即使只丢失其中一片数据也要重传整个数据报(这里既然有重传，说明传输层使用的是具有重传功能的协议，如TCP协议。而UDP协议不可靠，即使丢包了也不在意更不会重传，所以必须在应用层实现可靠通信的逻辑），这是因为IP本身没有重传机制，只能由更高层，比如传输层的TCP协议，来负责重传以确保可靠传输。于是，当来自同一个TCP报文段封装得到的原始IP数据报中的的某一片丢失后，接收端TCP迟迟接受不到完整报文段，它就会认为该报文段(包括全部IP分片)已丢失，TCP之后由于超时会收到三个冗余ACK就会重传整个TCP报文段，该报文段对应于一份IP数据报，可能有多个IP分片，但没有办法单独重传其中某一个数据分片，只能重传整个报文段。

## 联系
---
分片或分段发生的根源都在于MTU这一数据链路层限制，由于更靠近数据链路层的IP层在感知MTU方面相比于传输层具备天然的优势，在大小超过MTU的大数据报传输问题出现伊始，IP层分片技术就成为主流解决方案。而分片带来的诸多开销(额外首部、复杂处理逻辑)以及其甚至可能在端到端的传输过程中多次发生在网络转发设备(路由器)的问题，让网络协议设计者们又要费尽心思地在端到端通信过程中避免IP分片。

TCP分段技术被提出后，在一定程度上减少了IP分片，但正如上一节末尾所言，TCP分段仅是在发送端避免了IP分片，但是却不能保证在整个端到端通信路径上不会发生IP分片，因为路径上经常会有MTU值小于该TCP连接协商得到的MTU值的链路，在转发至该段链路之前转发设备仍需分片，所以说TCP分段并不能完全避免IP分片。

那么如何才能彻底避免分片呢？

答案其实不难想到：如果能在TCP连接双方正式通信之前，就能通过某种方式预先知道端到端路径的MTU，即路径中包含的各条链路的MTU最小值(称为路径MTU，**Path MTU**)，这一预先获知路径MTU的过程，称为路径MTU发现(**Path MTU Discovery**)，这样此后每次通信都会基于此MTU推导得到的MSS值在发送方TCP层处执行分段。

路径MTU发现如何实现呢？

大家都记得IP首部中有三个标志位，第一位预留，第二位**DF(Don't Fragment)**，第三位**MF(More Fragments)**。其中**DF**如果为1，意思是这个IP数据报在传输的过程中不能分片，如果此IP数据报大于网络接口的MTU，请直接丢弃，并发送消息告诉源主机已丢包。什么消息呢？**ICMP**的消息，告诉包因为太大了，因为不能分片所以丢弃了，并告诉源主机请重新发送不超过MTU的数据报，那发什么样的ICMP消息呢？再回顾一下ICMP首部结构，有一个**Type**字段，还有**Code**字段，发送**Type=3, Code=4, MTU=该接口的MTU值X**的消息既可以了。当这个ICMP消息到达IP数据报的源主机，源主机知道原来是IP数据报太大了，那最大可以发送多大的包呢？ICMP消息里有，那就是MTU=丢弃该包的网络接口的MTU值X，于是源主机再次发送不超过MTU=X的数据报就可以避免在传输路径上的IP分片。

有人会说，如果这个大小不超过X的IP数据报在传输过程中又遇到更小的MTU怎么办？重复以上步骤即可~

路径MTU发现看似完美避免了IP分片的问题，但同时又带来了新的问题：如果ICMP消息最终没能到达源主机怎么办？很显然该IP数据报就被静静丢弃了，TCP连接超时而被断开。ICMP为什么回不来？一般是被防火墙或路由器的访问控制列表(Access Control List, ACL)给无情拒绝了。如果你可以管理并配置这些设备，只要允许ICMP **Type=3, Code=4** 的消息可以通过即可，否则只有老老实实关闭路径MTU发现功能了，因为至少分片还能通信，而避免分片则彻底无法通信了...

## 实例
---
参见[**2**](https://blog.csdn.net/yusiguyuan/article/details/22782943)与[**3**](http://www.cnblogs.com/diegodu/p/4647644.html)

## 总结
---
1. (TCP)分段和(IP)分片各自发生在不同的协议层(分段-TCP传输层，分片-IP层)
2. 切分的原因不尽相同(数据量超出上限，分段应用数据上限-MSS，分片上限-MTU)
3. 虽然分段和分片不会在发送方同时发生，但却可能在同一次通信过程中分别在发送主机(分段)和转发设备(分片)中发生
4. 发送端采用的传输层协议不同(分段-TCP，分片-UDP/ICMP...)
5. 分段仅可能发生在发送端，分片不仅可能发生在发送端，更还可能发生在路径上任何一台工作在三层或以上的设备中，而两者的重组都只会发生在接收端

## 参考资料
---
1.[网络协议-TCP分段与IP分片](https://blog.csdn.net/ns_code/article/details/30109789)
2.[TCP层分段与IP层分片的关系](https://blog.csdn.net/yusiguyuan/article/details/22782943)
3.[IP分片浅析](http://www.cnblogs.com/diegodu/p/4647644.html)
4.[TCP常见选项](http://www.cnblogs.com/lshs/p/6038494.html)
5.[TCP/IP协议栈中为什么选择IP层负责分片](https://www.zhihu.com/question/22181709)
6.[为什么IP层要分片而TCP层要分段？](https://blog.csdn.net/stpeace/article/details/74152332)
7.[动手学习TCP-数据传输](https://www.cnblogs.com/wilber2013/p/4853674.html)
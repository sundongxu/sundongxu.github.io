---
title: 转发表(MAC表)、ARP表、路由表总结 
categories: 
- Network
tags:
- Forwarding 
- Routing
- ARP
---

## 全文概要
计算机网络中一个关键步骤在于通信路径上不同节点对于流经本节点的数据包转发，常见的交换设备主要是交换机(第二层、三层)和路由器(第三层)，在实际运行时，它们各自维护一些表结构帮助完成数据包的正确寻址与转发，本文详细介绍了三张至关重要的表：转发表、ARP表与路由表的在网络数据包转发功能中发挥的作用，以及它们协同工作的原理，顺便也会接着[**之前的文章**](http://dongdongdong.me/2017/10/30/Network/Hardware/relay/)继续谈谈交换机和路由器的一些事儿。

<!--more-->

## 网络分层协议
---
计算机网络是将地理上隔离的计算节点从物理上相连(双绞线、光纤、无线信号等)，并采用分层方式，将计算机网络自顶向下依次分为五层：应用层->传输层->网络层->数据链路层->物理层，对应设计有一整套网络协议栈，各层次分别运行有多种协议，下层向紧邻上层提供服务并隐藏通信细节，上层调用紧邻下层提供的服务，完成主机不同层次之间的对等通信，高层(应用层、传输层、网络层)之间借此实现逻辑相连通信。之所以称为“逻辑通信”，是因为不同主机相同的较高层次之间好像是沿水平方向传送数据，但事实上这两个对等层次之间并没有一条水平方向的物理连接，举例说明如下：

1. **传输层协议**用于实现**应用进程间**逻辑通信，这是因为**进程**本来就是便于资源分配与管理而发明的**逻辑概念**，并不是**物理实体**，无法从物理上产生关联；
2. **网络层协议**用于实现**主机间**逻辑通信，这是因为两个**通信主机**通常位处网络拓扑中的不同位置，大多时候并非直接从物理上相连，之间需要一条根据**路由协议**选路确定的通信路径，网络层协议负责数据包最终能够发送给目的主机，但向上(即给运输层)只提供尽力而为(Best-Effort)的数据包服务；

3. **数据链路层**用于实现**点到点通信**，由于不同节点在物理上通过真实的数据链路相连，不能再算是逻辑通信。

## 转发表(Forwarding Table)
---
### MAC地址
MAC地址(Media Access Control Address)，即媒体访问控制地址，通常也称为以太网地址或物理地址，它是一个用于确认网络设备位置的地址，每个网络设备(如网络适配器(Network Adapter)，即网卡(Network Interface Card, NIC))都有世上唯一的MAC地址，一台设备若有多张网卡，则每个网卡都必须具有一个唯一的MAC地址，这个是在网络设备出厂时由厂商烧制确定。

MAC地址共48位，即6个字节，通常每4位构成一个16进制数，从而可以表示成xx:xx:xx:xx:xx:xx的形式，每个x都是一个16进制数。其中ff:ff:ff:ff:ff:ff为广播地址，以此作为目的地址的数据包会被交换机广播至全部端口，发到与其端口相连的全部局域网;01:xx:xx:xx:xx:xx是多播地址。

### 工作流
转发表，又称MAC表，聊到它就不得不提到交换机里，因为交换机就是根据转发表来转发数据帧的。

交换机本质上也是一个计算机，拥有计算(CPU)、存储(SRAM或TCAM)和网络资源(转发芯片和链路)，甚至还会安装专用操作系统，它会维护一张记录着局域网主机端口MAC地址与交换机端口对应的表，交换机就是根据这张表负责将数据帧传输到指定的主机端口上的。

交换机具有“**存储转发**”功能：

* 交换机在接收到数据帧以后，首先会记录数据帧中的源MAC地址和对应的到达端口到MAC表中，这一过程通常称为“自学习”，不需要任何的人工干预；

* 接着，交换机检查自己的MAC表是否有数据帧中目的MAC地址的匹配条目，如果有，则会根据MAC表中记录的对应端口将数据帧转发出去，这一转发方式称为“单播”(Unicast)。而如果没有，则会将该数据帧从非达到端口的其它全部端口发送出去，这一转发方式程序称为“广播”(Broadcast)。

下面会以图示的方式详细讲解交换机传输数据帧的过程，下面先来看看单个交换机转发的情形：

{% qnimg Hardware/forwarding-single-switch.png %}

步骤如下：
1. 主机A会将一个源MAC地址为本机网卡物理地址，目的MAC地址为主机B网卡物理地址的数据帧发送给交换机1；
2. 交换机收到此数据帧后，首先将数据帧中的源MAC地址和对应的输入端口0记录到交换机维护的MAC地址表中；
3. 然后交换机会检查自己的MAC地址表中是否有数据帧中的目的MAC地址的信息，如果有，则从MAC地址表中记录的对应端口发送出去，如果没有，则会将此数据帧从非接收端口的所有端口发送出去，这里仅从端口1发出；
4. 这时，局域网的所有主机(通过交换机相连的全部主机)都会收到此数据帧，但是只有主机B收到此数据帧时会响应这个广播帧，并回应一个数据帧(这个回应由什么机制确定的，需要再查资料确定一下～TCP的ACK包？)，此数据帧中包括主机B网络设备的MAC地址；
5. 当交换机收到主机B回应的数据帧后，也会记录数据帧中的源MAC地址，即主机B网络设备的MAC地址，这时，再当主机A和主机B相互通信时，交换机就根据MAC地址表中的记录，实现单播了，一趟转发流程实际交换机就“学习”到了两个转发表条目。

那么当局域网内存在多个交换机互连的时候，交换机的MAC地址表是如何记录的呢？下图就展示了该种情形：

{% qnimg Hardware/forwarding-multiple-switch.png %}

步骤如下：
1. 主机A将一个源MAC地址为本机网卡物理地址，目的MAC地址为主机C网卡物理地址的数据帧发送给交换机1；
2. 交换机1收到此数据帧后，会学习源MAC地址，并检查MAC地址表，发现没有目的MAC地址的记录，则会将数据帧广播出去，主机B和交换机2都会收到此数据帧；
3. 交换机2收到此数据帧后也会将数据帧中的源MAC地址和对应的端口记录到MAC地址表中，并检查自己的MAC地址表，发现没有目的MAC地址的记录，则会广播此数据帧，主机C和主机D都会收到此数据帧；
4. 主机C收到数据帧后，会响应这个数据帧，并回复一个源MAC地址为本机网卡物理地址的数据帧，该帧最终会送往主机A，这时交换机1和交换机2都会将主机C的MAC地址记录到自己的MAC地址表中，并且以单播的形式将此数据帧发送给主机A；
5. 这时，主机A和主机C通信就可以以单播的形式传输数据帧了，A与D、B与C及B与D的通信与上述过程一样，因此交换机2的MAC地址表中记录着主机A和主机B的MAC地址都对应其端口3。

### 小结
从以上两幅图可以看出，交换机具有动态自学习源MAC地址与物理端口映射的功能，并且交换机的一个端口可以对应多个MAC地址，但是一个MAC地址只能对应一个端口。

注：交换机动态学习的MAC地址默认只有300s的有效期，如果300s内记录的MAC地址没有对应的通信过程来更新对应条目，则会自动删除此记录，这是由交换机中的一个计时器所维护的。

## ARP表(Address Resolution Table)
---
### ARP协议
首先明确一点，**在以太网环境下，同一个网段的主机之间需要知道对方的MAC地址，才能进行通信**。

上一节介绍了交换机的工作原理，了解到交换机是根据MAC寻址，查表确认输出端口以完成本节点转发任务的。看到这里其实应该可以抛出从一开始就被我们忽视了的问题：在初始构造数据包准备发送时，源主机究竟要如何获得目的主机网络设备MAC地址的呢？这时，就需要使用到ARP协议。在网络拓扑中的每个节点或说主机上，实际都维护有一张ARP表，它记录着主机的IP地址(网络地址)到MAC地址(物理地址)的映射关系。

ARP协议，即地址解析协议，它是一个网络层协议，运行在各网络节点上，负责完成主机IP地址到MAC地址的映射。

### 工作流
接下来根据下图，详细讲解一下ARP协议的工作原理：

{% qnimg Hardware/arp-protocol.png %}

步骤如下：
1. 如果主机A想发送数据包给同一网段内的另一台主机B(通过交换机相连的节点处于同一网段)，很明显，A的用户应用程序要么已经知道B的IP地址，或者说域名(Domain Name，DNS协议会完成主机名到IP地址的映射，这里不是重点)，那么主机A首先会检查自己的ARP缓存表(ARP Cache)，查看是否有主机B的IP地址与其MAC地址的对应关系，如果有，则直接将主机B网络设备的MAC地址作为目的MAC地址封装到数据帧中，无需进一步操作即获取到数据帧封装所需的全部信息，此后完成封装并发送数据帧到目的MAC地址。如果没有，主机A则会发送一个ARP请求信息(ARP Request)，请求的目的IP地址是主机B的IP地址，目的MAC地址是MAC层的广播地址(即ff:ff:ff:ff:ff:ff)，源IP地址和MAC地址是主机A的IP地址及其MAC地址；
2. 当交换机接收到此数据帧之后，发现此帧是广播帧，因此，会将此数据帧从非接收的所有端口发送出去；
3. 同一网段中的所有节点都会接收到该ARP请求数据包，目的IP不匹配的节点会直接忽略该请求，而当主机B接收到此数据帧后，解析到IP地址与自己的IP地址一致，先将主机A的IP地址及其MAC地址的对应关系记录到自己的ARP缓存表中，同时会发送一个ARP应答(ARP Response)，应答数据包源MAC地址即主机B自身网络设备的MAC地址，该应答通过交换机转发至主机A；
4. 主机A在收到这个回应的数据帧之后，在自己的ARP缓存表中记录主机B的IP地址和MAC地址的对应关系。而此时主机A已经可以继续封装准备发往主机B的数据帧，而交换机也已经学习到了主机A和主机B的MAC地址与其端口的对应关系，之后主机A发送的数据帧通过交换机转发至主机B。

**这里有一点值得注意的是**传输过程中IP地址与MAC的地址的变化问题：
* MAC地址在同一个广播域中传播时始终不变，但在跨越广播域(即经过路由器)时，会由于重新封装而改变，源MAC地址将变为路由器的输出端口的MAC地址，目的MAC地址随网络拓扑实际情况而定，若路由器与目的主机所在网段直连，此时目的MAC地址就是目的主机的MAC地址；而不管是源IP地址还是目的IP地址，在数据包传输过程中都始终不会改变。

<!--主机A在构造数据帧准备发送给主机B时，源IP地址和源MAC地址分别是自己的IP地址和MAC地址，但是目的IP地址和目的MAC地址则应该是主机B的对应地址，但是由于A与B不是直接通过链路相连，通信路径上还需经过若干转发设备，如交换机，因为MAC层即数据链路层只负责链路两端点之间的数据传输，且交换机通常具有“存储转发”功能，即其在接收到一个数据包后会将其短暂存储，历经解包和再次打包成帧(Framing)的过程，根据其上维护的MAC表查到对应目的MAC地址的输出端口将其转发。此时转发出去的数据帧有效载荷(Payload, 数据帧=以太网头部+网络层数据报，即帧有效载荷实际就是网络层数据报)不会改变，这是因为交换机工作在数据链路层，在解析数据包的时候不会涉及到网络层及以上层次的协议字段。但是由于重新打包步骤的存在，导致以太网头部中的源MAC地址将会被交换机修改为其自身的硬件MAC地址，目的MAC地址则依然不会改变，之后从输出端口推向链路进行传输。-->


### 小结
网络中每个节点都会通过运行ARP协议而维护节点内部的一张ARP缓存表，用于完成从IP地址到MAC地址的映射，在发送数据之前往往会先查询本地ARP表中对应目的IP地址的MAC地址，若没有表项则会发起ARP广播请求直至获取对应主机响应并发来应答，将该应答中包含的目的IP地址与MAC地址的映射关系添加到ARP缓存表之后，在数据链路层就可以以该MAC地址为目的MAC地址封装数据帧并发送。

## 路由表(Routing Table)
---
### IP地址
IP地址(Internet Protocol Address)，即互联网协议地址，也称为网络层地址或主机地址，是分配给网络上的各个网络设备的地址。

现在流行的IP协议有两个版本：IPv4(Internet Protocol Version 4)和IPv6(Internet Protocol Version 6)。其中，IPv4地址为32位，即4个字节，为便于使用，常以xxx.xxx.xxx.xxx每个字节8位从二进制表示为十进制数，这种表示方法称为点分十进制，地址可分为A、B、C、D、E五大类，32位全为1的IP地址：255.255.255.255称为“受限广播地址”(Limited Broadcasr Destination Address)，用于将一个分组以广播方式发送给本网络中的所有主机，路由器则阻挡该分组通过，将其广播功能限制在本网内部，因此可以说路由器隔离了广播域(交换机隔离了冲突域)。

随着网络规模和节点数量的不断扩展，出于32位的IPv4将很快被分配使用殆尽的担忧，又推出了IPv6地址，128位，16个字节，通常每4位表示为一个16进制数，16个字节分为8组，每组包含2个字节即4个16进制数，组与组之间以冒号分割：ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff为IPv6协议下的广播地址。

### IP VS MAC

其实MAC地址和IP地址本质上都是网络设备物理端口的性质，都可以用于寻址网络设备，但如果它们功能类似，只是工作的层次不同的话，两者之间应该可以通过某种机制实现相互替换，但为什么它们共存至今呢？

对于MAC地址和IP地址的比较，有人曾这么比喻：一个人已经有了手机号(IP地址)，为什么还要有身份证号呢(MAC地址)？身份证号是一个人的唯一标识号，只要有这个号码就能找到该人，但是为啥他的朋友们不用身份证号来寻找该人而用手机号呢？对，因为方便。但是如果该人犯罪，警察用手机号呼叫寻找该人，傻子才会接吧...而且换了号码怎么办？那不就得用你的身份证号在全国发布通缉令，利用身份证号在全国各种系统范围(消费、社交、医疗)内搜索该人的记录，才能最终定位该人实施有效逮捕嘛。

上述例子实际就契合了两种地址存在的意义：IP地址是逻辑地址，根据网络协议，在不同的地理位置加入互联网就会分配到完全不同的IP地址(DHCP动态分配IP地址)，但由于属于网络层，相对较高的抽象层次设计的初衷就是为了简化通信，方便使用，尤其对于用户进程而言；MAC地址是物理地址，工作在数据链路层，一旦出厂时由厂商确定并烧制入网络设备的EPROM中就具有了固定的全球唯一的地址，任何时候任何条件都不会改变，虽说使用起来不太方便，且描述的是较低层的数据链路通信细节，但在任何时候都可用于数据通信寻址。

更严谨完善的解释如下：

> 问题：既然每个以太网设备在出厂时都有一个唯一的MAC地址了，那为什么还需要为每台主机再分配一个IP地址呢？或者说为什么每台主机都分配唯一的IP地址了，为什么还要在网络设备（如网卡，集线器，路由器等）生产时内嵌一个唯一的MAC地址呢？

> 解答：主要原因基于以下几点：
> （1）IP地址的分配是根据网络的拓扑结构，而不是根据谁制造了网络设置。若将高效的路由选择方案建立在设备制造商的基础上而不是网络所处的拓扑位置基础上，这种方案是不可行的；
> （2）当存在一个附加层的地址寻址时，设备更易于移动和维修。例如，如果一个以太网卡坏了，可以被更换，而无须取得一个新的IP地址。如果一个IP主机从一个网络移到另一个网络，可以给它一个新的IP地址，而无须换一个新的网卡；
> （3）无论是局域网，还是广域网中的计算机之间的通信，最终都表现为将数据包从某种形式的链路上的初始节点出发，从一个节点传递到另一个节点，最终传送到目的节点。数据包在这些节点之间的移动都是由ARP协议负责将IP地址映射到MAC地址上来完成的。
    
下面再通过一个例子看看IP地址和MAC地址是怎样结合起来传送数据包的：
> 假设网络上要将一个数据包（名为PAC）由北京的一台主机（名称为A，IP地址为IP_A，MAC地址为MAC_A）发送到纽约的一台主机（名称为B，IP地址为IP_B，MAC地址为MAC_B）。这两台主机之间不太可能是直连起来的，因而数据包在传递时必然要经过许多中间节点(如路由器，网关服务器等)，假定在传输过程中要经过C1、C2、C3(其输入、输出端口的MAC地址分别为M1_In/M1_Out，M2_In/M2_Out，M3_In/M3_Out)三个节点。A在将PAC发出之前，先发送一个ARP请求，找到其要到达IP_B所必须经历的第一个中间节点C1的到达端口MAC地址M1_In，然后在其数据包中封装地址：IP_A、IP_B，MAC_A和M1_In。当PAC传到C1后，再由ARP根据其目的IP地址IP_B，找到其要经历的第二个中间节点C2的到达端口MAC地址M2_In，然后再封装目的MAC地址为M2_Out的数据包传送到C2。如此类推，直到最后找到IP地址为IP_B的B主机的MAC地址MAC_B，最终传送给主机B。在传输过程中，数据包源IP地址IP_A、目的IP地址IP_B不变，而源MAC地址和目的MAC地址，由于中间节点重新封装数据帧而不断改变，直至目的地址MAC地址为MAC_B，数据包最终到达目的主机B。

综上所述，IP地址和MAC地址相同点是它们都可以作为设备地址标识，不同的特点则主要有以下几个方面：
1. 对于网络上的某一设备，如一台计算机或一台路由器，其IP地址可变（但必须唯一），而MAC地址不可变。我们可以根据需要给一台主机指定任意的IP地址，如我们可以给局域网上的某台计算机分配IP地址为192.168.0.112 ，也可以将它改成192.168.0.200。而任一网络设备（如网卡，路由器）一旦生产出来以后，其MAC地址永远唯一且不能由用户改变；
2. 长度不同。IP地址为32位4字节，MAC地址为48位6字节；
3. 分配依据不同。IP地址的分配是**基于网络拓朴**，MAC地址的分配是**基于制造商**；
4. 寻址协议层不同。IP地址应用于OSI第三层，即网络层，而MAC地址应用在OSI第二层，即数据链路层。 数据链路层协议可以使数据从一个节点传递到同一段链路的另一个节点上(通过MAC地址寻址)，而网络层协议使数据可以从一个网络传递到另一个网络上(ARP根据目的IP地址，找到中间节点的MAC地址，通过中间节点转发，从而最终到达目的网络)。

### 工作流
路由器负责不同网段(Subnet, 子网)之间的通信，每个与路由器端口相连的网络被称为一个子网或网段，也就是一个广播域。在路由器中也有一张表，这张表叫做路由表，通过在网络节点上运行路由协议，记录并更新去往不同网段的路径信息。路由表中的信息分为直连路由和非直连路由：

* **直连路由**：直接连接到路由器端口的网段，该信息由路由器自动生成；
* **非直连路由**：不是直接连接到路由器端口的网段，此记录需要手动添加或使用动态路由生成。

本机Linux系统下, 双网卡em1(114.212.84.179)与virbr0(192.168.122.1)，执行命令：route -n，获取内核IP路由表如下：

<style>
table th:nth-of-type(1) {
    width: 100px;
}
</style>

<style>
table th:nth-of-type(2) {
    width: 100px;
}
</style>

<style>
table th:nth-of-type(3) {
    width: 100px;
}
</style>

|  Destination  |   Gateway    |    Genmask    | Flags | Metric | Ref | Use | Iface  |
| ------------- | ------------ | ------------- | ----- | ------ | --- | --- | ------ |
| 114.212.80.0  | 0.0.0.0      | 255.255.248.0 | U     | 100    | 0   | 0   | em1    |
| 192.168.122.0 | 0.0.0.0      | 255.255.255.0 | U     | 0      | 0   | 0   | virbr0 |
| 0.0.0.0       | 114.212.80.1 | 0.0.0.0       | UG    | 100    | 0   | 0   | em1    |

解析上述路由表：
> 第一项：目的网络为114.212.80.0/21，网关地址为0.0.0.0，这表示该网段属于与路由器某端口直连的网段，数据包将从路由器em1接口输出；

> 第二项：目的网络为192.168.122.0/24，网关地址同样为0.0.0.0，表示该网段属于与路由器某端口直连的网段，数据包将从路由器virbr0接口输出；

> 第三项：目的网络为0.0.0.0时，即匹配任意网段，由于路由表匹配执行最先匹配策略，第三项就是当目的IP地址无法与前两项匹配时的成功匹配项，对应的网关称为默认网关，即路由器中没有存储某个目的网络的表项时应该转发至的下一跳地址，从em1端口输出。

路由器中记录的条目有的需要手动添加，称为静态路由；有的则是动态获取的，称为动态路由。表中的每个条目都有以下属性：

1. **目的网络地址(Destination)**：网络地址和网络掩码相与的结果用于定义本机可以达到的目的网络范围，通常情况下，目的网络范围包含以下几种情况：
    (1) **主机地址**：某个特定主机的网络地址；
    (2) **子网地址**：某个特定子网的网络地址；
    (3) **默认路由**：所有未在路由表中指定的网络地址，用0.0.0.0统一匹配，用于配置默认网关；
    
2. **网络掩码(Genmask)**：又称为子网掩码(Subnet Mask)，是一个32位地址，作用是将一个同样也是32位的IPv4地址划分成网络地址(Network Address)和主机地址(Host Address)。子网掩码不能单独存在，它必须结合IP地址一起使用。子网掩码是用来判断任意两台主机是否处于同一网段的根据，简单来说就是两台主机各自的IP地址与本机配置的子网掩码做按位与操作，如果结果相同，则说明这两台主机是处于同一网段，可以进行直接的通讯，而无需路由器的转发；

3. **网关(Gateway，又被称为下一跳服务器(Next Hop Server))**：发送IP数据包时，网关定义了针对特定的网络目的地址，数据包将要被发送到的下一跳IP地址。如果是与路由器直接相连的网段，网关通常就是路由器对应的网络端口的IP地址，但是此时接口必须与网关一致。如果是远程网络或默认路由，网关通常是与路由器相连网络上的某个服务器或路由器。如果目标是本主机所属网络，不需要路由，网关显示为"\*";

4. **接口(Iface)**：接口定义了针对特定的网络目的地址，路由器用于转发数据包的网络接口(路由器的物理端口)。网关必须位于和接口相同的子网(默认网关除外)，否则造成在使用此路由项时需调用其他路由项，从而可能会导致路由死锁;

5. **跳数(Metric)**：跳数用于指出路由的成本，通常情况下代表到达目标地址所需要的总跳数，一个跳数代表经过一个路由器，IP数据报首部中的TTL字段就是该数据报所能存活的总跳数。跳数越少往往代表着该路由成本越低，跳数越多则说明成本越高。当具有多条达到相同目的网络的路由选项时，路由算法会选择具有更少跳数的路由。

6. **标志(Flags)**：多种路由表项标记含义如下：
    (1) U：路由是动态的；
    (2) H：目标是一个主机；
    (3) G：路由指向网关；
    (4) R：恢复动态路由产生的表项；
    (5) D：由路由的后台程序动态安装；
    (6) M：由路由的后台程序修改；
    (7) !：拒绝路由。
    
7. **引用次数(Refs)**：Linux内核中未使用，一般是0；

8. **查找次数(Use)**：此路由项被路由软件查找的次数。

路由器是工作在网络层的，在网络层可以识别逻辑地址，即IP地址，也就是说数据包解析时最多可将数据帧拆包成IP数据包，路由器无法操作数据报的载荷字段，但是可以针对IP首部做些事情：当路由器的某个端口收到一个包时，路由器就会读取包中的目地IP地址，然后在路由表中进行查找。如果在路由表中找到目的IP地址对应条目，则把包转发到路由器的对应端口。如果没找到，那么如果路由器配置默认路由(默认网关)，就默认将所有无法解析的目的网段主机的数据包都先发往该默认网关做进一步转发，如果没有配置默认路由，则将该包丢弃，并返回源主机以不可达(Unreachable)的信息。这就是数据包路由的过程。

利用下图详细介绍路由器的工作原理：

{% qnimg Hardware/routing-mac-frame.png %}

步骤如下：
1. 主机A在网络层将来自上层的报文封装成IP数据报，IP首部中的源IP地址为自己的IP地址，目的IP地址为主机B的IP地址。主机A会用本机配置的24位子网掩码与目的地址进行“与”运算，得出目的地址与本机不在同一个网段(主机A位于192.168.1.0/24网段，主机B位于192.168.2.0/24网段，或称属于不同子网)，因此发送给主机B的数据包需要经过网关路由器1的转发；
2. 主机A通过ARP请求获得网关路由器1的E0端口的MAC地址，并在数据链路层将路由器E0端口的MAC地址封装成以太网帧首部中的目的MAC地址，源MAC地址是自己的MAC地址，随后发送数据帧给路由器1；
3. 路由器1从端口E0接收到该数据帧，随后执行解析，将数据链路层的首部剥落去掉，并在路由表中检查是否有目的IP地址的网段对应表项(即192.168.2.2/24和其所在192.168.2.0/24网段)，根据路由表中记录，发往192.168.2.0/24网段(中主机)的数据包的下一跳(Next Hop)或称网关地址为10.1.1.2/8(实际是路由器2的E1端口的IP地址)，而路由器发现下一跳地址正好位处与自己E1端口直连的网段(10.0.0.0/8)，于是数据在路由器1的E1端口重新封装，此时，以太网帧的源MAC地址是路由器1的E1端口的MAC地址，目的MAC地址则是路由器2的E1端口的MAC地址，通过ARP广播得到，封装完毕发送数据帧给路由器2；
4. 路由器2从端口E1接收到该数据帧，随后执行解析，将数据链路层的首部剥落去掉，对目的IP地址进行检测，并与路由表进行匹配，此时发现目的主机IP地址所在网段正好是自己E0端口的直连网段，路由器2于是通过ARP广播，获知主机B的MAC地址，此时数据包在路由器2的E0端口再次封装，源MAC地址是路由器2的E0端口的MAC地址，目的MAC地址是主机B的MAC地址，随后发送数据帧给主机B；
5. 完成以上1~4，主机B终于接收到来自主机A的数据包。

综上，看似较为“简单”的跨网段主机通信，真也不是太容易的一件事儿。

### 小结
路由表负责记录一个网络到另一个网络的路径，路由器依赖路由协议及其确定的路由表完成三层，即网络层的数据转发工作。路由表项中最重要的信息在于目的网段和网关，即下一跳IP地址的对应关系，网关通常是专门的网关服务器或者路由器，而网关会负责将该数据包最终转发至目的网段。

## 参考资源
---
[1] [详解网络传输中的三张表，MAC地址表、ARP缓存表以及路由表](http://dengqi.blog.51cto.com/5685776/1223132)
[2] [单播、多播(组播)和广播的区别](http://blog.csdn.net/wangerge/article/details/3931491)
[3] [路由表详解](http://blog.csdn.net/zyboy2000/article/details/50528253)
[4] [传输层协议概述](http://www.ywnds.com/?p=1269)
[5] [MAC、IP和路由传输封装过程](http://blog.csdn.net/u014113117/article/details/51311837)
---
title: 网络测量之NetSight(NSDI-2014)
date: 2018-05-18 15:00:00
categories:
- Network
tags:
- Measurement
- NSDI
- End-Host
- Packet-Level
---

## 全文概要
[**NSDI 2014**](https://www.usenix.org/conference/nsdi14)年中，由斯坦福大学"SDN之父"，**Nick Mckeown**教授带领的实验室发表了题为《I Know What Yout Packet Did Last Hop: Using Packet Histories to Troubleshoot Networks》的论文。基于本文设计的**NetSight**系统，运行在端系统之上，并利用已有的**OpenFlow**交换机的功能特性，实现了**Packet-Level**的网络数据监测。本文尝试概述了该系统的核心思想，作为网络测量领域相关工作的一次备忘。
<!--more-->

## Paper
---
Proposed in [NSDI.2014](https://www.usenix.org/conference/nsdi14)
-[**《I Know What Yout Packet Did Last Hop: Using Packet Histories to Troubleshoot Networks》**](https://www.usenix.org/system/files/conference/nsdi14/nsdi14-paper-handigol.pdf)

## Idea
---
在本文提出的**NetSight**系统中，在网络中的每一个数据包流经的每一跳都会为该数据包产生一个称作**Postcard**的日志条目(包含Switch ID、输出端口号以及转发状态的版本号)，并发送至服务器集群，在集群内部实现负载均衡(压缩->洗牌->解压)，对于某个数据包，其**Postcard**最终会被发送至同一个服务器进行处理，在该服务器处，关于此数据包的全部**Postcard**会被根据网络拓扑信息组装成为**Packet History**并作持久化存储，它将可以回答所有关于此数据包在网络中的经历的问题。网络用户基于**NetSight**提供的API开发用于调试的各类应用程序，将其所关注的网络事件或行为通过文中提出的**Packet History Filter(PHF)**进行表达，并在已有的**Packet History**处执行查询匹配(分为实时的(Live)和历史的(Historical))，以获知网络事件是否发生。

## Terminology
---
① **Packet History** 定义如下：
> The route a packet takes through a network plus the switch state and header mofification it encounters at each hop.

即**数据包历史**，包含该数据包流经网络采用的路由(路径)，和它在经过每一跳(交换机)时，交换机的状态以及在本跳受到的首部修改情况。
**Packet History**反映了该**Packet**在网络中的“种种遭遇”，它提供给我们如下信息：
- **What** the packet looked like as it entered the network (Headers)
- **Where** the packet was forwarded (Switches + Ports)
- **How** it was changed (Header Modification)
- **Why** is was forwarded that way (Matched Flow/Actions + Flow Table)

② **Packet History Filter(PHF)**：一种类似正则表达式的语言，可十分方便简洁地为所关注的数据包历史指定需匹配的 **数据包路径**、**交换机状态**和**数据包首部字段**。

③ **Postcard**：每个数据包在流经每个交换机时，都会生成一个称作 **Postcard** 的事件记录，它是包括数据包首部、交换机ID、输出端口号以及交换机转发状态的版本号等信息的一个拷贝。最终，同一个数据包流经各个交换机时产生的**Postcard**都在同一台处理服务器处，根据事先知道的网络拓扑信息，重新组装成为一个关于此数据包的**Packet History**，用于匹配用户查询。

④ **Postcard Filter(PF)**：**PHF**语言中的原子元素(最小元素)，用于在某一跳处匹配一个数据包。**PF**实际上是以下字段过滤器的联合：数据包首部、交换机ID(数据路径ID，dpid)、输入端口号(inport)、输出端口号(outport)和该数据包“遭遇”的交换机状态(用一个版本号表示，version)。**PF**可写作如下形式：

```
--bpf [not] <BPF> --dpid [not] <switch ID> --inport [not] <input port> -- outport [not] <output port> --version [not] <version>
```

**BPF**是Berkeley Packet Filter**表达式。**[not]**是可选项，表示当前字段不可取后面的值。一个**PF**至少应该包含上述字段的中的一个。

例如：现要匹配一个数据包，它的源IP为A，中途经过交换机S，其中输入端口不能是P。对应PF可写作如下：

```
--bpf "ip src A" --dpid S --inport not P
```

## Challenge
---
1. **Path Visibility**，即**路径可见性**，我们必须以某种方式观察到并记录每个数据包采用的路径
2. **State Visibility**，即**状态可见性**，我们必须重新构建每个数据包在每一跳观察到的准确的交换机状态
3. **Modification Visibility**，即**(首部)修改可见性**，我们必须知道数据包在何处以及如何被修改
4. **Collection Scale**，即**信息收集可扩展性**，上述功能都必须在最大网络速率环境中正常运行
5. **Storage Scale**，即**信息存储可扩展性**，在某些时候，提供**Packet History**查询功能需要存储几乎所有信息
6. **Processing Scale**，即**信息处理可扩展性**，查询处理必须与信息收集与存储的速度相匹配

## Application
---
### ndb: Interactive Network Debugger
---
**ndb**是一种交互式的网络调试工具，允许网络应用开发者基于异常网络事件设置相应的**PHF**，一旦事件发生，返回得到的**Packet History**将会包含一系列的造成该异常事件的交换机转发事件，从而帮助诊断如下常见问题：
1. Reachability Error(可达性错误)
2. Race Condition(竞争条件)
3. Incorrect Packet Modification(不正确的数据包修改)

### netwatch: Live Invariant Monitor
---
**netwatch**允许运维人员以不变体(不变式，invariant)的形式指明其所关注的网络行为，并在某数据包违反该不变式时触发警告。
**netwatch**本质上是一个不变式的库，包含了各种用**PHF**写就的不变式，用于匹配违反不变式的数据包。一旦对应的**PHF**被推送至**NetSight**系统中并被下发配置完成后，回调(callback)就会将违反此不变式行为的**Packet History**返回。
**netwatch**现能支持以下网络不变式：
1. Isolation(通信区域隔离)
2. Loop Freedom(无环路径)
3. Waypoint Routing(路径点路由)
4. Max Path Length(最大路径长度)

### netshark: Network-wide Path-Aware Packet Logger
---
**netshark**允许用户对整个**Packet History**设置过滤器，包括它们在每一跳的路径和首部字段值。
**netshark**接受一个来自用户定义的**PHF**，返回匹配该查询的**Packet History**，它还包括一个**wireshark**组件用于解析收集到的结果。用户能够观察到某一跳处的数据包属性(包头字段值、交换机ID、输入/输出端口号和其所匹配的流表的版本号)，以及**Packet History**的属性(所属路径、所属路径长度等)。

### nprof: Hierachial Network Profiler
---
**nprof**帮助用户分析任意链路集合，以理解造成当前链路利用率情况的流量特征和路由决定。
**nprof**会将收集到的**Packet History**与拓扑信息组合起来已提供一个生动的分层视图，表明是哪些交换机在发送流量给当前链路，以及流量大小。

## Achitecture
---
{% qnimg Network/Measurement/related-work-netsight/architecture.png %}

**NetSight**使用一个**Central Coordinator**来管理多个**Worker(NetSight Server)**。**NetSight Application(应用程序)**将基于**PHF**的钩子(Trigger)和查询发送至**Coordinator**，后者则将匹配得到的**Packet History**返回给前者。

**Coordinator**设置从**Switch**到**NetSight Server**的**Postcard**传输和从网络控制平面到**Coordinator**的**State Change Record(状态变化记录)**传输。

最后，**Coordinator**会对其所管理的**Worker**们执行周期性的存活检测、查询与钩子的广播，并在组装**Packet History**时与之交换拓扑信息。

## Work Flow
---
### Postcard Generation
---
> Goal: Record all information relevant to a forwarding event and send for analysis.

当一个数据包经过一个交换机时，交换机通过复制(**Duplicate**)该包生成一个**Postcard**，随后将其裁剪(**Truncate**)至最小包大小(**Minimun Packet Size**)，使用相关状态对其进行标记(**Marking**)，并将其转发(**Forwarding**)至**NetSight Servers**进行分析。

### Postcard Collection
---
> Goal: To send all postcards for a packet to one server, so that its packet history can be assembled.

为了重建某一个数据包的**History**，**NetSight**需要在单个**Server**中收集到与该数据包相关的所有**Postcard**(一组**Postcard**)。而为了扩展集群的处理能力，**NetSight**需要确保这些**Postcard**组在集群服务器之间实现负载均衡，通过对数据包的**Flow ID**，即五元组(5-tuple:Src/Dest IP, Src/Dest Port, Protocol)进行哈希操作来确保局部性(同一个流的数据包的**Postcard**会被最终发往同一个服务器进行处理)。

为实现负载均衡而执行的**洗牌(Shuffle)**过程，是基于时间划分的，每个时间段称作一个**Round**，只有在每一个**Round**末尾，服务器会将在这个**Round**时间段内收集到的**Postcard**发送至其通过**Shuffle**过程确定的最终目的地服务器，在那里等待被装配。

而在**Shuffle**之前，利用相同流和相邻流之间的数据包在头部字段的冗余，还可实现**Postcard**的压缩处理，进一步减小**Shuffling**过程中的带宽开销。

### History Assembly
---
> Goal: To assemble packet histories from out-of-order postcards.

在某台**NetSight**服务器上，由于从交换机发来数据不同的传播与排队时延(Propagation and Queuing Delay)，同一个数据包的**Postcards**很有可能是乱序到达的，在装配成**History**之前，**NetSight**会利用拓扑信息而非时间戳来将属于各个数据包的**Postcards**排好序。

一旦某**NetSight**服务器从所有其它服务器上收到了某个数据包在全部**Round**时段的**Postcards**，它将会对这些**Postcards**执行解压缩，并将其合并至一个称作**Path Table**的数据结构中，该结构帮助将同一个数据包的全部**Postcards**组成成为一个组(**Group**)。为了标识某个数据包的全部**Postcards**，**NetSight**使用数据包中从不会被更改的头部字段，如**IP ID**、**Fragment Offset**和**TCP Sequence Number**等，组合成为一个新字段，叫作**Packet ID**，它可用来唯一标识某个流中的一个数据包。**Path Table**简单地采用**Packet ID**作为索引(**Key**)，而对应的值(**Value**)则为此数据包对应的全部**Postcard**组成的列表。

**NetSight**服务器随后会提取这些已组织好的**Postcard**组，一次提取一组，即一个数据包的全部**Postcard**信息，将其组装成为该数据包的**Packet History**。对于每一组，**NetSight**使用**Switch ID**和**Output Port**，以及实际的拓扑信息，对其执行拓扑排序。而最终排序好的**Postcard**列表即该数据包的**Packet History**。

### Filter Trigger
---
> Goal: To immediately notify applications of fresh packet histories matching a pre-installed PHF.

一旦某个数据包的**Packet History**被组装完成，**NetSight**会将其与所有由如**netwatch**等应用程序预先安装下发的(活跃的、实时的)**Live PHF**进行匹配，并在成功匹配某项后立即触发向对应应用程序的通知。

### History Achival
---
> Goal: To efficiently store the full set of the packet histories.

在每一个**Round**中生成的**Packet History**的数据流将被写入到一个文件。在写入持久化存储的过程中，**NetSight**会采用与之前提到的一样的压缩算法，都是利用了同一个数据包的**Postcard**之间的与同个流中的数据包之间的头部信息冗余，来实现压缩以减小存储开销。

### Histort Queries
---
> Goal: To enable applications to issure PHF queries against archived packet histories.

当一个应用程序下发对于某一具体时间范围内的历史性的**Historical PHF**查询需求时，该查询会在所有**NetSight**服务器上并行执行。在这里，由于压缩帮助提高了有效磁盘吞吐率，从而也缩短了查询完成时间。

## Implementation
---
**NetSight**实现分为两个过程：
1. 插入到**OpenFlow Controller**与**Switches**之前的**Control Channel Proxy(控制通道代理)**，用于记录配置更改，使用Python实现。
2. **NetSight Server**，用于执行全部的**Postcard**与**History**相关操作，使用C++实现。

### Flow Table State Recorder
---
在SDN环境下，任何网络状态的更改都是由控制器参与和协调完成的，这就提供了一个监控与截获交换机配置更改的绝佳地点。**NetSight**在控制器与**OpenFlow**交换机的控制路径上，实现了一个透明代理，称作**Flow Table State Recorder**，每当控制器向交换机们下发新的流表项，**Recorder**都会截获此信息并存储到数据库中。

具体来说，**Recorder**工作流程如下：对于每条由控制器发送给交换机的**OpenFlow**规则，即**OpenFlow**流表项，**Recorder**都会将其截获并存储，同时在该规则对应的**Action**操作中追加一个新**Action**，令交换机除了对其所接收到匹配该规则的数据包进行正常转发之外，还为其生成一个**Postcard**并发送至**NetSight**服务器。这些追加的**Action**实际就是指示交换机创建一个匹配本条目的数据包的一份拷贝，并用当前交换机ID、输出端口号以及所匹配的流表项的版本号对其进行标记。其中，流表项的版本号就是一个简单的计数器，随着每条流修改信息的到来而递增。这些标记会覆盖原数据包的目的MAC地址字段，因此此数据包，实际就是**Postcard**无需像原数据包一样转发，只需发送到指定**NetSight**服务器处即可，因此目的MAC地址实际就没有作用了。一旦该**Postcard**被生成，随后会基于一个单独的VLAN字段值转发至对应的**NetSight**服务器。**Postcard**可采用两种网络转发模式，通过一个单独的网络处理即**Out-of-band(带外)**传输，和在普通网络中传输即**In-band**传输。当采用后者时，交换机会通过一个特殊的VLAN字段标记识别出**Postcard**，从而避免为**Postcard**再生成**Postcard**，造成不必要的带宽开销。

### ＮetSight Server
---
作为实际的**End-host(端系统)**测量解决方案，本文提出的**NetSight**系统并没有涉及对交换机的修改，转而充分利用了**OpenFlow**交换机提供的已有特性，相比**Flow Table State Recorder**仅仅追加规则使得交换机能够生成**per-packet**的**Postcard**，整个**NetSight Server**集群才是整个**NetSight**系统的核心，毕竟是它完成了全部**Postcard**的收集、组装、存储和查询的任务。
---
title: Linux内核调优参数对比与解释
date: 2018-05-20 15:00:00
categories:
- Operating System
tags:
- Linux
- Kernel
- Tuning
---

## 全文概要
本文介绍了Linux系统性能优化点常见的内核参数含义及其调优方式，以供学习参考。
<!--more-->

具体情况要具体分析，此处只是收集通常会利用到的一些参数的调整，做一些简单的对比和解释。
全部系统变量默认值测试均在 **Ubuntu 16.03** 系统下进行，内核版本为**4.13.0-42-generic**。

## Net
---
```
net.ipv4.tcp_syncookies=1
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_syncookies
```

默认值：1
作用：是否打开SYN Cookie功能，该功能可以防止部分SYN攻击

---

```
net.ipv4.ip_local_port_range=1024  65535
```

查询命令：
```
cat /proc/sys/net/ipv4/ip_local_port_range
```

默认值：32768   60999
作用：可用端口的范围

---

```
net.ipv4.tcp_fin_timeout=30
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_fin_timeout
```

默认值：60(s)
作用：TCP挥手阶段FIN报文超时时间

---

```
net.ipv4.tcp_timestamps=1
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_timestamps 
```

默认值：1
作用：TCP时间戳

---

```
net.ipv4.tcp_tw_recycle=0  # Ubuntu系统下未找到该变量
```

查询命令：

```
cat /proc/sys/net/ipv4/tcp_tw_recycle
```

默认值：0
作用：针对TIME-WAIT，不要开启。不少文章提到同时开启tcp_tw_recycle和tcp_tw_reuse，会带来C/S在NAT方面的异常。个人接受的做法是，开启tcp_tw_reuse，增加ip_local_port_range的范围，减小tcp_max_tw_buckets和tcp_fin_timeout的值(参考[**这里**](http://ju.outofmemory.cn/entry/91121)和[**这里**](http://www.cnblogs.com/lulu/p/4149312.html))

---

```
net.ipv4.tcp_tw_reuse=1
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_tw_reuse
```

默认值：0
作用：针对TIME-WAIT，做为客户端可以启用(例如，作为nginx-proxy前端代理，要访问后端的服务)

---

```
net.ipv4.tcp_max_tw_buckets=262144
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_max_tw_buckets 
```

默认值：32768
作用：针对TIME-WAIT，配置其上限。如果降低这个值，可以显著的发现time-wait的数量减少，但系统日志中可能出现如下记录：
```
kernel: TCP: time wait bucket table overflow."
```

对应的，如果升高这个值，可以显著的发现time-wait的数量增加。综合考虑，保持默认值。

---

```
net.ipv4.tcp_max_orphans=3276800
```

查询命令：

```
cat /proc/sys/net/ipv4/tcp_max_orphans 
```

默认值：32768
作用：orphans的最大值

---

```
net.ipv4.tcp_max_syn_backlog=819200
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_max_syn_backlog
```

默认值：256
作用：增大SYN队列的长度(SYN_REVD状态的连接个数)，容纳更多连接

---

```
net.ipv4.tcp_keepalive_intvl=30
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_keepalive_intvl 
```

默认值：75
作用：TCP保活机制，探测失败后，间隔若干秒后重新探测

---

```
net.ipv4.tcp_keepalive_probes=3
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_keepalive_probes 
```

默认值：9
作用：TCP保活机制，探测失败后，最多尝试探测几次

---

```
net.ipv4.tcp_keepalive_time=1200
```

查询命令：

```
cat /proc/sys/net/ipv4/tcp_keepalive_time 
```

默认值：7200
作用：TCP保活机制，间隔多久发送1次keepalive探测包

---

```
net.netfilter.nf_conntrack_tcp_timeout_established=600
```

查询命令：
```
cat /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_established
```

默认值：432000
作用：设置 conntrack tcp 状态的超时时间，如果系统出现下述异常时要考虑调整：

```
ping: sendmsg: Operation not permitted.
kernel: nf_conntrack: table full, dropping packet.
```

参考[**这里**](http://www.linuxidc.com/Linux/2012-11/75151.htm)还有[**这里**](http://blog.csdn.net/dog250/article/details/9318843)

---

```
net.netfilter.nf_conntrack_max=655350
```

查询命令：
```
cat /proc/sys/net/netfilter/nf_conntrack_max
```

默认值：262144
作用：设置 conntrack 的上限，如果系统出现下述异常时要考虑调整：

```
ping: sendmsg: Operation not permitted.
kernel: nf_conntrack: table full, dropping packet.
```

参考[**这里**](https://blog.yorkgu.me/2012/02/09/kernel-nf_conntrack-table-full-dropping-packet/)还有[**这里**](http://www.cnblogs.com/mydomain/archive/2013/05/19/3087153.html)

---

```
net.core.netdev_max_backlog=500000
```

查询命令：
```
cat /proc/sys/net/core/netdev_max_backlog
```

默认值：1000
作用：网卡设备队列长度(请求个数)

---

```
net.core.somaxconn=65536
```

查询命令：
```
cat /proc/sys/net/core/somaxconn
```

默认值：128
作用：已经成功建立连接等待被应用程序接受(accept调用)的(ESTABLISHED)队列长度(等待被接受的连接个数)

---

```
net.core.rmem_default=8388608
```

查询命令：
```
cat /proc/sys/net/core/rmem_default
```

默认值：212992
作用：默认的TCP数据接收窗口大小字节数

---

```
net.core.wmem_default=8388608
```

查询命令：
```
cat /proc/sys/net/core/wmem_default
```

默认值：212992
作用：默认的TCP数据发送窗口大小字节数

---

```
net.core.wmem_max=16777216
```

查询命令：
```
cat /proc/sys/net/core/wmem_max
```

默认值：212992
作用：最大的TCP数据发送窗口大小字节数

---

```
net.core.rmem_max=16777216
```

查询命令：
```
cat /proc/sys/net/core/rmem_max
```

默认值：212992
作用：最大的TCP数据接收窗口大小字节数

---

```
net.ipv4.tcp_mem=94500000   915000000   927000000
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_mem
```

默认值：93381  124511  186762
作用：内存使用的下限  警戒值  上限

---

```
net.ipv4.tcp_rmem=4096   87380   16777216
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_rmem
```

默认值：4096    87380   6291456
作用：socket接收缓冲区内存使用的下限  警戒值  上限

---

```
net.ipv4.tcp_wmem=4096   16384   16777216
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_wmem

```

默认值：4096    16384   4194304
作用：socket发送缓冲区内存使用的下限  警戒值  上限

---

```
net.ipv4.tcp_thin_dupack=1
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_thin_dupack # Ubuntu系统中未找到此变量
```

默认值：0
作用：收到dupACK时要去检查tcp stream是不是thin(less than 4 packets in flight)

---

```
net.ipv4.tcp_thin_linear_timeouts=1
```

查询命令：
```
cat /proc/sys/net/ipv4/tcp_thin_linear_timeouts
```

默认值：0
作用：重传超时后要去检查tcp stream是不是thin(less than 4 packets in flight)

---

```
net.unix.max_dgram_qlen=30000
```

查询命令：
```
cat /proc/sys/net/unix/max_dgram_qlen
```

默认值：512
作用：UDP队列里数据报的最大个数

---

针对lvs，关闭网卡LRO/GRO功能。现在大多数网卡都具有LRO/GRO功能，即网卡收包时将同一流的小包合并成大包 (tcpdump抓包可以看到>MTU 1500bytes的数据包) 交给内核协议栈。LVS内核模块在处理 >MTU 的数据包时，会将其丢弃。因此，如果我们用LVS来传输大文件，很容易出现丢包，传输速度慢。

解决方法，关闭LRO/GRO功能，命令：
```
ethtool -k eth0          #查看LRO/GRO当前是否打开
ethtool -K eth0 lro off  #关闭GRO
ethtool -K eth0 gro off  #关闭GRO
```

---

## Kernel
---
```
kernel.randomize_va_space=1
```

查询命令：
```
cat /proc/sys/kernel/randomize_va_space
```

默认值：2
作用：内核的随机地址保护模式

---

```
kernel.panic=1
```

查询命令：
```
cat /proc/sys/kernel/panic
```

默认值：0
作用：内核panic时，1秒后自动重启

---

```
kernel.core_pattern=core_%e
```

查询命令：
```
cat /proc/sys/kernel/core_pattern
```

默认值：|/usr/share/apport/apport %p %s %c %d %P
作用：程序生成core时的文件名格式

---

```
kernel.sysrq=0
```

查询命令：
```
cat /proc/sys/kernel/sysrq
```

默认值：176

作用：是否启用sysrq功能

---

## VM
---

```
vm.min_free_kbytes=901120
```

查询命令：
```
cat /proc/sys/vm/min_free_kbytes 
```

默认值：67584
作用：保留内存的最低值

---

```
vm.panic_on_oom=1
```

查询命令：
```
cat /proc/sys/vm/panic_on_oom 
```

默认值：0
作用：发生oom时，自动转换为panic

---

```
vm.min_free_kbytes=1048576
```

查询命令：
```
cat /proc/sys/vm/min_free_kbytes 
```

默认值：67584
作用：保留最低可用内存

---

```
vm.swappiness=20
```

查询命令：
```
cat /proc/sys/vm/swappiness 
```

默认值：60
作用：数值(0-100)越高，越可能发生swap交换

---

## File System
---

```
fs.inotify.max_user_watches=8192000
```

查询命令：
```
cat /proc/sys/fs/inotify/max_user_watches
```

默认值：524288
作用：inotify的watch数量

---

```
fs.aio-max-nr=1048576
```

查询命令：
```
cat /proc/sys/fs/aio-max-nr
```

默认值：65536
作用：aio最大值

---

```
fs.file-max=1048575
```

查询命令：
```
cat /proc/sys/fs/file-max
```

默认值：804894
作用：文件描述符的最大值

---

## 参考资料
---
[1][Linux内核高性能优化【生产环境实例】](http://yangrong.blog.51cto.com/6945369/1567427)
[2][linux内核参数解释说明](http://yangrong.blog.51cto.com/6945369/1321594)
[3][tcp_tw_reuse、tcp_tw_recycle 使用场景及注意事项](http://www.cnblogs.com/lulu/p/4149312.html)
[4][linux内核调优参数对比和解释](http://blog.51cto.com/nosmoking/1684114)

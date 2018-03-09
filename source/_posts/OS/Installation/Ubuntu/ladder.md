---
title: Shadowsocks 搭梯全教程
date: 2018-01-31 11:11:11
categories:
- Operating System
tags:
- VPN
- Shadowsocks
- Amazon EC2
- Proxychain
- Privoxy
- SwitchyOmega
---

## 全文概要
---
正为各种 [**VPN**](https://zh.wikipedia.org/wiki/Vpn) 被封或是网上那些不太靠谱的翻墙教程所困扰？

刚好，这里有一份 [**Shadowsocks**](https://zh.wikipedia.org/wiki/Shadowsocks) 指南，包含 **Linux** 和 **MacOS** 系统下的安装与配置，以及在 **浏览器** 和 **终端** 下翻墙的姿势介绍。

没错，这很可能就是你见过的最完整、最易懂的VPN搭建教程。
<!--more-->

## 前面的话
---
{% qnimg OS/Installation/Ubuntu/ladder/gfw.png %}

在当前这个知识爆炸的时代，作为一名学术研究者或技术钻研者，搜索引擎无疑是最好的学习工具，而 [**Google**](https://www.google.com)正是个中翘楚。但早在互联网广泛流行于中国之前，出于一些不可抗原因，[**GFW**](https://zh.wikipedia.org/wiki/%E9%98%B2%E7%81%AB%E9%95%BF%E5%9F%8E) 横空出世，国外网站对于内地网民从此遥不可及。好在技术无极限，更无国界，防火墙挡住了我们通往外界的视野，搭个梯子翻过去便是。

于是，科学上网正成为当今知识分子的必备技能。而市场上已经有不少搭好的“梯子”，各类付费 **VPN** 琳琅满目，购买套餐后按照提示配置即可，方便快捷，但问题是不够稳妥安全。换句话说，购买这些 **VPN** 是比较危险的，很容易就被...你懂的。例如，笔者亲身经历过头天刚下决心买了某老牌付费 **VPN** 的两年会员，第二天就被封的...

所以最好的方法是购买小众 [**VPS**](https://zh.wikipedia.org/wiki/%E8%99%9A%E6%8B%9F%E4%B8%93%E7%94%A8%E6%9C%8D%E5%8A%A1%E5%99%A8) 搭建自己的海外服务器，防止被误伤。作为第一次捣鼓这些的新手，我是十分不建议上来就花个几十美金购买 **VPS** 的，最好的方案是找一个免费的服务器来练练手，那么，亚马逊就站出来了，他家的 [**Amazon Web Services**](https://amazonaws-china.com/cn/)，在一定条件下能免费使用一年。

当然，你必须有一张信用卡，没有的话，上某宝买张虚拟信用卡，操作也很easy的啦。

注：
> Virtual Private Server, 简称 VPS，虚拟专用服务器
> Virtual Private Network, 简称 VPN，虚拟专用网络

## Shadowsocks
---
### 前世今生
[**Shadowsocks**](https://github.com/shadowsocks/shadowsocks/tree/master) 是目前最好的科学上网方式，它的流量经过加密，所以没有流量特征不会被GFW阻断；关键是，它的实现原理也通俗易懂。

#### long long ago
在很久很久以前，我们访问各种网站都是简单而直接的，用户的请求通过互联网发送到服务提供方，服务提供方直接将信息反馈给用户。
{% qnimg OS/Installation/Ubuntu/ladder/gfw-before.png %}

#### when devil comes
然后有一天，**GFW** 就出现了，他像一个收过路费的强盗一样夹在了在用户和服务之间，每当用户需要获取信息，都经过了 **GFW**，**GFW**将它不喜欢的内容统统过滤掉，于是客户当触发 **GFW** 的过滤规则的时候，就会收到 **Connection Reset** 这样的响应内容，而无法接收到正常的内容。
{% qnimg OS/Installation/Ubuntu/ladder/gfw-now.png %}

#### ssh tunnel
聪明的人们想到了利用境外服务器代理的方法来绕过 **GFW** 的过滤，其中包含了各种 **HTTP** 代理服务、**Socks** 服务、**VPN** 服务… 其中以 **ssh tunnel** 的方法比较有代表性：
> 1) 首先用户和境外服务器基于 ssh 建立起一条加密的通道
2-3) 用户通过建立起的隧道进行代理，通过 ssh server 向真实的服务发起请求
4-5) 服务通过 ssh server，再通过创建好的隧道返回给用户

{% qnimg OS/Installation/Ubuntu/ladder/gfw-ssh-tunnel.png %}

由于 **ssh** 本身就是基于 **RSA** 加密技术，所以 **GFW** 无法从数据传输的过程中的加密数据内容进行关键词分析，避免了被重置链接的问题，但由于创建隧道和数据传输的过程中，**ssh** 本身的特征是明显的，所以 **GFW** 一度通过分析连接的特征进行干扰，导致 **ssh** 存在被定向进行干扰的问题。

#### shadowsocks
@[**clowwindy**](https://github.com/clowwindy) 大神分享并开源了他的解决方案

简单理解的话，**Shadowsocks** 是将原来 **ssh** 创建的 **socks5** 协议拆开成 **server** 端和 **client** 端，所以下面这个原理图基本上和利用 **ssh tunnel** 大致类似。

> 1、6) 客户端发出的请求基于 Socks5 协议跟 ss-local 端进行通讯，由于这个 ss-local 一般是本机或路由器或局域网的其他机器，不经过 GFW，所以解决了上面被 GFW 通过特征分析进行干扰的问题
2、5) ss-local 和 ss-server 两端通过多种可选的加密方法进行通讯，经过 GFW 的时候是常规的TCP包，没有明显的特征码而且 GFW 也无法对通讯数据进行解密 
3、4) ss-server 将收到的加密数据进行解密，还原原来的请求，再发送到用户需要访问的服务，获取响应原路返回

{% qnimg OS/Installation/Ubuntu/ladder/gfw-shadowsocks.png %}

### 基本原理
从源码角度分析，**Shadowsocks** 的工作原理并不复杂。**Shadowsocks** 包括 **local.py** 和 **server.py** 两个程序：**local** 运行在用户自己的机器上，**server** 运行在墙外的服务器上。正常工作模式下，**local** 通常会监听本地 **1080** 端口，提供 **socks5** 协议接口。其工作流程简述如下：

> 1. 在用户本机进程和 local 的 1080 端口建立 TCP 连接之后，首先会发送一个 hello 包，或称 handshake 握手包；
2. local 程序接收到这个包之后，进行简单的包数据检查之后就返回一个确认包；
3. 本机进程收到确认的包之后，会再发送一个请求包，包的主要内容就是目标服务的地址和端口号；
4. local 程序接收到请求包之后，会和 server 程序建立一个 TCP 连接，连接建立之后会把上面的目标服务的地址和端口号传给 server。这个连接是穿墙的关键，连接里面传输的数据都是经过加密的，最常用的就是 aes-256-cfb；
5. local 程序会对请求包返回一个确认包，告知本机进程其与 server 程序的连接已建立，等待数据请求的传输；
6. 然后本机进程就开始向 local 传输实际的数据，local 接收到之后加密继续传给 server；
7. server 接收到之后把数据解密出来，然后和目标服务建立 TCP 连接，并把解密后的数据请求发送出去；
8. server 接收到请求的数据后最终返回给本机进程就是上述的反过程。

### 实例图解
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocks-basics-cn.png %}

ss要求本机运行local.py，海外服务器运行server.py。local.py默认监听localhost的1080端口，该端口代理浏览器的请求。browser要访问google时，会和localhost:1080进行一次基于sock5协议的通信，如上图的红色虚线框，sock5协议可以去了解下，维基百科有不错的介绍。

代理的流程如下：
>1. localhost:1080经过sock5协议后，就知道要访问google了
2. local程序会把流量加密，然后把普通的TCP流量发往海外服务器；
3. 海外服务器收到请求后，解密得到要访问google
4. 海外服务器请求google后把得到的数据加密返回给local
5. local解密返回给browser。

**ss** 的解密和加密都基于用户设置的密码，所以 **local** 和 **server** 之间可以做到加密和解密的一致。

因此，**Shadowsocks** 的优点在于它通过流量混淆隐秘解决了 **GFW** 通过分析流量特征从而干扰的问题，这是它优于 **SSH** 和 **VPN** 翻墙的地方（但 **VPN** 更注重加密安全性）。缺点也依然明显，需要一点点技术和资源（墙外 **VPS服务器**）来搭建 **Shadowsocks服务**，好在已经有人搭建相应的服务出售翻墙服务了。当然你也完全可以搭建一个专属于自己的 **Shadowsocks** 服务，后文介绍了完整的安装与配置教程以供参考。

## Shadowsocks 服务器
---
从前文了解到，**Shadowsocks** 服务由 **local(客户端)** 和 **server(墙外服务器)** 组成，先来看看如何搭建个人墙外 **VPS**。

作为初学者，**Amazon EC2** 提供的免费海外服务器已经完全够用，本节介绍利用其搭建 **Shadowsocks** 服务器的步骤。

### 注册Amazon
点击[**此处**](http://aws.amazon.com/cn/) 右上角注册，一路下来资料照填，之前有亚马逊账号的话就直接登录。

需要注意的一点是：**注册过程需要绑定信用卡**。

说好是免费的，怎么还要绑定信用卡扣费呢？所谓免费，就是在你没有用超的情况下（作为 **Shadowsocks** 服务器，唯一能超限使用的就是网络流量，每月15G流量）。绑定信用卡应该会扣两笔钱，都是1美元，一笔是预授权，一笔应该是押金，1年后会返还，如果超限使用，顺便用着1美金抵扣。实在没有信用卡的学生党，到某宝上买一个一美元的虚拟信用卡，按要求输入店家发来的卡号、有效期，持卡人写“匿名”即可。

注册完会需要输入你的手机号，**AWS** 会拨通你的手机号，让你输入网页提示的一串数字即完成注册。

### 创建EC2实例
用刚才注册好的账号登录并进入 **AWS** 控制台，点击 **EC2 (云中的虚拟服务器)**，如果需要改变服务器节点(地区)的话，右上角可以选择：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-ec2.png %}

进入 **EC2** 管理控制台后，点击 **启动实例**：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-ec2-boot.png %}

上图中可以看到已经有一个正在运行的实例，那是我之前创建的不用在意，新用户的话，已运行实例数量应为0。

之后进入 **Amazon 系统映像(AMI)** 选择，我选的是 **Ubuntu Server 16.04 LTS(HVM)，SSD Volume Type**：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-ami-ubuntu.png %}

然后选择 **实例类型**，默认的 **符合条件的免费套餐** 即可，点击 **下一步：配置实例详细信息**：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-instance-type.png %}

**步骤3：配置实例** 和 **步骤4：添加存储** 都是默认选项，直接点下一步跳过，直至 **步骤5：添加标签**，新建标签，键为 **name**，值为 **ss-vpn**，其实都可以任意指定，之后 **创建密钥对** 时会用到，之后下一步：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-tag.png %}

**步骤6：配置安全组**，添加规则如图所示，其中端口 **8388** 是与 **Shadowsocks** 相关的，也可以自由指定：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-security-group.png %}

最后确定开始审核。这时候会提示生成密钥对，这个很重要，一定要保存好，没有这个密钥对是无法远程登录(如SSH)管理你的服务器的，下载密钥对 **.pem** 文件后才能启动实例：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-key-download.png %}

查看启动日志，发现 **EC2** 实例成功启动：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-boot-successful.png %}

### 连接到服务器
实例初始化完成后，就可以进行远程连接了，右键你的实例，点击连接。会弹出连接提示，如果你使用的是 **Windows**，可以查看使用 **PuTTY** 从 **Windows** 连接到 **Linux** 实例，如果你是 **Linux** 或者 **Mac OS**，可以直接通过 **SSH** 连接到你的服务器，具体可以查看亚马逊给出的文档 **使用 SSH 连接到 Linux 实例**。

记住下图中所示位置这个 **公有IP**，它是你的 **Shadowsocks** 的服务器IP：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-public-ip-dns.png %}

按弹窗提示 **SSH** 连接到远程实例：
{% qnimg OS/Installation/Ubuntu/ladder/ss-server-connect-ssh.png %}

### 配置Shadowsocks
**SSH** 连接到服务器之后，执行如下命令：
{% codeblock %}
$ sudo -s                                          # 获取超级管理员权限
# apt-get update                                   # 更新源
# apt-get install python-pip                       # 安装pip工具，用于安装shadowsocks
# pip install shadowsocks                          # 安装shadowsocks
# vim /etc/shadowsocks_conf.json                   # 编辑shadowsocks配置，内容见后文
# ssserver -c /etc/shadowsocks_conf.json -d start  # 启动shadowsocks服务器
{% endcodeblock %}

**shadowsocks_conf.json** 配置内容如下：
{% codeblock %}
{
    "server":"0.0.0.0",
    "port_password": {
        "[这里写你的端口号，通常为8388]": "[密码1]",
        "[其他端口号]": "[密码2]" # 一个其实就够了...可能这里也是可以多端口登录吧
    },
    "local_address": "127.0.0.1",
    "local_port":1080,
    "timeout":600,
    "method":"aes-256-cfb",
    "auth": true
}
{% endcodeblock %}

配置字段说明如下：

|      字段      |                 含义                |
| :-----------  | :---------------------------------- |
| server        | 服务端监听地址(IPv4或IPv6)             |
| server_port   | 服务端端口                            |
| local_address | 本地监听地址，缺省为443                 |   
| local_port    | 本地监听端口，一般为1080                |
| password      | 用以加密的密钥                         |
| timeout       | 超时时间(秒)                          |
| method        | 加密方法，默认为 aes-256-cfb           |
| fast_open     | 是否启用 TCP-Fast-Open, true or false |
| workers       | worker数量，仅在Unix和Linux下有效       |

 编辑完上述配置文件后，一定记得执行最后一条命令以启动 **Shadowsocks** 服务。

至此，**Shadowsocks** 服务器，即位于墙外的 **server** 配置全部完成。

那么 **local**，也就是客户端呢？别着急，下一节就是啦~

## Shadowsocks 客户端
---
### Linux
以 **Ubuntu** 为例，图形化客户端无疑最方便，推荐 [**Shadowsocks-qt5**](https://github.com/shadowsocks/shadowsocks-qt5/releases)，安装也十分简单：
{% codeblock %}
$ sudo add-apt-repository ppa:hzwhuang/ss-qt5
$ sudo apt-get update
$ sudo apt-get install shadowsocks-qt5
{% endcodeblock %}

安装成功之后，按 **win** 键输入关键字后应该能找到该客户端，如图所示：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocks-Qt5.png %}

启动客户端后，点击：connection -> add，新建到 **Shadowsocks Server** 的连接：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocks-Qt5-add-connection.png %}

在弹窗中填入早前配置好的 **Shadowsocks** 服务器信息：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocks-Qt5-server-config.png %}

保存设置后右键点击刚添加的条目，点击连接：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocks-Qt5-connect.png %}

到这里，在 **Ubuntu** 下已经启动 **local** 程序，尽管它已经与墙外的 **server** 建立起连接并可以完成通信，但是如浏览器、终端等需要访问外网的本机进程而言，却仍然无法达到翻墙成功的效果，这是因为这些本机进程在试图直接访问外网服务，而这些访问请求都会被 **GFW** 根据其特殊的流量特征而被屏蔽过滤掉，导致访问失败。

解决方案无疑就是 **Shadowsocks** ：浏览器、终端等本机进程需要以 **Shadowsocks** 为中间人将数据请求发送出去，即需要将发往被墙掉的外网服务的流量重定向到本地 **Shadowsocks** 客户端进程(**local**)。该进程默认监听本地端口 **1080**，因此需要将本机进程的流量发送至该端口上，这就涉及到特定进程的配置工作，主要是 **浏览器** 和 **终端**。

#### 浏览器
##### Chrome
配置代理，将对被墙的服务的流量重定向至本地 **Shadowsocks** 客户端，即 **local** 。**Chrome** 插件 **SwitchyOmega** 即可实现流量选择性代理，需要从官方插件商店中下载安装，操作过程中你很快会发现一个问题：访问官方商店这一行为本身就需要翻墙...

好在 **Chrome** 还有另一款插件 [**谷歌访问助手**]() ，下载安装后浏览器即可访问 **Google** 相关服务，安装完成后在浏览器右上角插件栏可以看到：
{% qnimg OS/Installation/Ubuntu/ladder/google-assistant-icon.png %}

看到这里你或许又有了问题：那为啥不干脆就使用 **谷歌助手** 插件翻墙就好了，还要 **SwitchyOmega** 干啥？

答案是：**谷歌助手** 只能让浏览器可以访问 **Google** 相关服务，如搜索、学术、地图、商店等等，但是其余被墙网站如 **Youtobe**、**Facebook** 和 **Twitter** 等仍然不可访问，而 **SwitchyOmega** 则帮助我们可以通过浏览器访问到世界上任何网站，再也不用受 **GFW** 的拦截。

下载安装好后，开始配置 **SwitchyOmega** 之前，首先需禁用 **谷歌访问助手**，原因是两个插件本质都是代理功能，会发生冲突，要想使 **SwitchyOmega** 生效，就必须先禁用冲突项 **谷歌访问助手**：
{% qnimg OS/Installation/Ubuntu/ladder/google-assistant-disable.png %}

然后启用 **SwitchyOmega**：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-enable.png %}

之后浏览器右上角插件栏出现一个黑色圆圈图标，点击并选择 **选项**：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-icon.png %]}

之后进入 **SwitchyOmega** 配置页面：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-main.png %}

新建名为 **ss** 的情景模式，当然名称也可以自己起：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-new-profile.png %}

保存配置后是这样：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-ss.png %}

在 **情景模式** 下选择 **auto switch**：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-autoswitch.png %}

点击 **添加规则列表**，将以下链接粘贴至网址框：
> https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt

并一定完全按下图配置：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-autoswitch-config.png %}

点击 **立即更新情景模式**，此时由于未保存修改会弹出如下窗口：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-confirm.png %}

点击 **应用选项**，**规则列表正文** 框内显示出 **gfwlist.txt** 文件中包含的过滤域名列表：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-gfwlist-head.png
 %}

搜索关键字 **google** 会找到对应过滤项：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-gfwlist-google.png %}

往下翻还能找到很多平时“可望不可即“的站点，如 **Facebook**、**Youtube**、**Vimeo**、**Steam**...甚至 **Tensorflow**、**GitHub** 还有 **Android** ... 这个恕我不是太能理解，学习都不让嘛...

保存修改后，点击浏览器右上角的 **SwitchyOmega** 插件图标，选中 **auto switch** 模式：
{% qnimg OS/Installation/Ubuntu/ladder/switchy-omega-finish.png %}

测试相关被墙网站，成功访问，搞定！

##### Firefox
网上很多人推荐代理插件 **FoxyProxy**，配置过程参见[**此篇**](http://www.aichengxu.com/linux/11086629.htm)，不过我反正搞了很久没弄好，只好放弃...

下面教你个简单好用的方法，为 **FireFox** 配置 **Shadowsocks** 的非全局代理：

先安装 **Genpac**，可以用 **pip** 安装：
{% codeblock %}
$ sudo pip install genpac
{% endcodeblock %}

新建 **Shadowsocks** 文件夹存放 **pac** 文件：
{% codeblock %}
$ mkdir ~/shadowsocks
$ cd shadowsocks
{% endcodeblock %}

生成 **pac** 文件：
{% codeblock %}
$ genpac --proxy="SOCKS5 127.0.0.1:1080" --gfwlist-proxy="SOCKS5 127.0.0.1:1080" -o autoproxy.pac --gfwlist-url="https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt"
{% endcodeblock %}

设置系统网络代理为 **Automatic**，即自动模式，配置 **URL** 则填入本地 **pac** 文件地址：
> file:///home/sundongxu/shadowsocks/autoproxy.pac

具体见下图：
{% qnimg OS/Installation/Ubuntu/ladder/network-proxy-automatic.png %}

最后点击在系统范围内应用：**Apply System Wide**，并按要求输入密码修改这一设置。

之后配置 **Firefox** 使用系统代理，进入浏览器 **Preferences(偏好设置)**：
{% qnimg OS/Installation/Ubuntu/ladder/firefox-preference-proxy.png %}

{% qnimg OS/Installation/Ubuntu/ladder/firefox-use-system-proxy.png %}

至此，**Ubuntu** 下的 **Firefox** 浏览器也应该能够成功翻墙啦~

#### 终端
相比浏览器而言，**Ubuntu** 下的终端翻墙则要容易得多，推荐 **proxychains**，安装配置极为简单：

**proxychains** 安装：
{% codeblock %}
$ sudo apt install proxychain
{% endcodeblock %}

**proxychains** 配置：
编辑配置文件 **/etc/proxychains.conf**：
{% codeblock %}
$ sudo vim /etc/proxychains.conf
# 注释最后一行：socks4 127.0.0.1 9050
# 添加一行：socks5 127.0.0.1 1080
{% endcodeblock %}

**proxychains** 测试：
{% codeblock %}
$ proxychains curl www.google.com
{% endcodeblock %}

**proxychains** 使用：
用命令行启动软件，在前面加上 **proxychains**，如：
{% codeblock %}
$ proxychains firefox
{% endcodeblock %}

使用 **Shadowsocks** + **proxychains** 代理打开新的 **firefox** 实现浏览器翻墙。当然，最方便的就是直接输入 **proxychain bash/zsh** 新建一个新的 **Shell**，在新 **Shell** 中运行的所有命令都将使用代理成功翻墙。

### MacOS
---
同样推荐使用带 **GUI** 的 **Shadowsocks** 客户端，最好用的当属 [**ShadowsocksX-NG**](https://github.com/shadowsocks/ShadowsocksX-NG/releases/)，下载 **zip** 文件后直接安装即可。

安装完成后会在 **Launchpad** 中找到其应用图标：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocksX-NG.png %}

点击启动应用，**Mac** 桌面右上角会出现一个**纸飞机**图标，点击该图标，选择：服务器 -> 服务器设置：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocksX-NG-add-server.png %}

之后在窗口中填入早先配置好的 **Shadowsocks** 服务器信息：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocksX-NG-server-config.png %}

点击确定后保存配置。再次点击**纸飞机**图标，选择 **PAC自动模式** 或 **全局模式**，然后点击打开 **Shadowsocks** 启动本客户端，之后就可以愉快在 **Mac** 上翻墙啦~

#### 终端
使用过程中不知道你够不够细心，因为完成上述配置后，浏览器确实可以成功访问外网，但是终端却仍然无法翻墙，这里推荐一款 **Mac** 下终端翻墙利器：[**privoxy**](https://www.google.com.hk/search?q=privoxy&oq=privoxy&aqs=chrome.0.0j69i61j0l4.2324j0j7&sourceid=chrome&ie=UTF-8)，原理就不讲了，直接上配置教程：

**privoxy** 安装：
很简单，用 **brew** 安装即可：
{% codeblock %}
$ brew install privoxy
{% endcodeblock %}

**privoxy** 配置：
打开配置文件 **/usr/local/etc/privoxy/config**，加入下面两个配置项：
{% codeblock %}
listen-address 0.0.0.0:8118
forward-socks5 / localhost:1086 .
{% endcodeblock %}

第一行设置privoxy监听任意IP地址的 **8118** 端口。第二行设置本地socks5代理客户端端口 **1086**，可以通过 **ShadowsocksX-NG** 的设置页面确定，见下图：
{% qnimg OS/Installation/Ubuntu/ladder/shadowsocksX-NG-settings.png %}

**privoxy** 启动：
{% codeblock %}
$ sudo /usr/local/sbin/privoxy /usr/local/etc/privoxy/config
{% endcodeblock %}

查看是否启动成功：
{% codeblock %}
$ netstat -na | grep 8118
{% endcodeblock %}

查询结果如下则表明启动成功：
{% qnimg OS/Installation/Ubuntu/ladder/privoxy-listen.png %}

如果没有，可以查看日志信息，判断哪里出了问题。打开配置文件找到 **logdir** 配置项，查看 **log** 文件。

**privoxy** 使用：
在命令行终端中输入如下命令后，该终端即可翻墙了：
{% codeblock %}
$ export http_proxy='http://localhost:8118'
$ export https_proxy='http://localhost:8118'
{% endcodeblock %}

他的原理是讲socks5代理转化成http代理给命令行终端使用。

如果不想用了取消即可：
{% codeblock %}
$ unset http_proxy
$ unset https_proxy
{% endcodeblock %}

如果关闭终端窗口，功能就会失效，如果需要代理一直生效，则可以把上述两行代码添加到 **~/.bash_profile** 文件最后(或任意**Shell**配置文件，我用的就是 **.zshrc**)：
{% codeblock %}
vim ~/.bash_profile
# 将以下内容追加到配置文件末尾
export http_proxy='http://localhost:8118'
export https_proxy='http://localhost:8118'
{% endcodeblock %}

使配置立即生效：
{% codeblock %}
$ source ~/.bash_profile # source ~/.zshrc
{% endcodeblock %}

还可以在 **~/.bash_profile(或我的.zshrc)** 里加入开关函数，使用起来更方便
{% codeblock %}
function proxy_off(){
    unset http_proxy
    unset https_proxy
    echo -e "已关闭代理"
}

function proxy_on() {
    export no_proxy="localhost,127.0.0.1,localaddress,.localdomain.com"
    export http_proxy="http://127.0.0.1:8118"
    export https_proxy=$http_proxy
    echo -e "已开启代理"
}
{% endcodeblock %}

最后，**Mac**下终端翻墙的流程为：
> 打开 ShadowsocksX-NG -> 启动 privoxy -> 执行命令：proxy_on -> 终端成功翻墙

自此，全文终。

## 参考资料
---
[1][VPN搭建 - Amazon EC2 + Shadowsocks](http://www.tyrion.wang/2017/02/04/VPN%E6%90%AD%E5%BB%BA-%E4%BA%9A%E9%A9%AC%E9%80%8AEC2-Shadowsocks/)
[2][写给非专业人士看的Shadowsocks简介](https://vc2tea.com/whats-shadowsocks/)
[3][Shadowsocks源码分析——协议和结构](https://loggerhead.me/posts/shadowsocks-yuan-ma-fen-xi-xie-yi-yu-jie-gou.html)
[4][使用亚马逊AWS搭建免费的Shadowsocks服务器](http://celerysoft.github.io/2016-01-15.html)
[5][我的Linux入门之路 - 02.Shadowsocks-Qt5配置](http://blog.csdn.net/xienaoban/article/details/54772942)
[6][Ubuntu使用Shadowsocks-qt5科学上网](https://www.mystery0.vip/2017/01/12/Ubuntu%E4%BD%BF%E7%94%A8Shadowsocks-qt5%E7%A7%91%E5%AD%A6%E4%B8%8A%E7%BD%91/)
[7][Mac命令行终端下使用shadowsocks翻墙](http://www.cashqian.net/blog/001486989831982332565298e4942a2bb8f56b08f9d2475000)
[8][各版本Shadowsocks-GitHub](https://github.com/shadowsocks/shadowsocks/wiki/Ports-and-Clients#windows%20SS%E5%85%A8%E5%B9%B3%E5%8F%B0%E5%90%84%E4%B8%AA%E7%89%88%E6%9C%AC)
[9][Ubuntu下设置Shadowsocks的非全局代理（PAC自动代理）](http://blog.leanote.com/post/sxdeveloper/Ubuntu%E4%B8%8B%E8%AE%BE%E7%BD%AEShadowsocks%E7%9A%84%E9%9D%9E%E5%85%A8%E5%B1%80%E4%BB%A3%E7%90%86%EF%BC%88PAC%E8%87%AA%E5%8A%A8%E4%BB%A3%E7%90%86%EF%BC%89)
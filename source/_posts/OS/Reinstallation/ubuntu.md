---
title: 记一次 Ubuntu 16.04 重装前后
date: 2018-01-15 11:11:11
categories:
- Operating System
tags:
- Linux
- Ubuntu
- Reinstallation
---

## 全文概要
前几天实验室PC网络功能出Bug，有线网连不上nju登录站点，而加载内核驱动模块后的无线网卡却妥妥的没有问题，本来懒癌发作打算用一世无线网...可又想到之后虚拟机中的虚拟网卡NAT上网会有坑，且无线网本身速度不够稳定，考虑再三，拉上[**Quber**](http://www.cnblogs.com/burningTheStar/)一起“愉快”地开始了找坑之旅。然而，嗯...虽然折腾的结果并不总是那么令人振奋，但付出总有回报。谨以此文纪念本人各种努(chui)力(si)尝(zheng)试(zha)，以及此后愤而重装的心路历程。
<!--more-->

## 重装的前夜
---
### 挣扎是徒劳的
遇事儿的第一反应总是想解决问题，只不过每每碰上超出能力范围的...
不！还是要挣扎一下，万一成功了呢！

以下梳理一下Bug出现的前因后果：
> 硬件配置：
> （1）**操作系统**：Ubuntu 16.04
> （2）**内核版本**：4.13.0-26-generic
> （3）**有线网卡**：Realtek RTL8111/8168/8411   驱动：r8169
> （4）**无线网卡**：TPLINK RTL8812   驱动：[**rtl8812**](https://github.com/gnab/rtl8812au)

> Bug现象：有线网无法连接到**校园网登录门户**，当然就不能上网了。

> 可能原因：
>（1）**硬件故障**
> 	 ① 网线坏了？ ---毕竟不是一次两次了...
	   ② 网口坏了？ ---学子莘莘，独坑我一人？
> 	 ③ 网卡坏了？ ---可能性比我写程序出Bug大大大多了...
>（2）**软件故障**
> 	 ① 网卡驱动不匹配？ ---可为啥之前都是好的...
> 	 ② 内核中除了网卡驱动之外的网络相关模块故障？ ---“可为啥之前是好的”二连...我干了啥？
>（3）**DNS配置错误**
>	 ① /etc/resolve.conf
>	 ② /etc/resolveconf.d/base
>	 ③ /etc/resolveconf.d/head

哦对，忘记说了，出事儿之前我正在配置[**P4环境**](https://www.sdnlab.com/19912.html)，编译安装了一堆东西，会不会...嗯，下次还是在虚拟机里试试，这个回头再说...

以上，在我的水平范围内委实只能想到这么多，毕竟...知识总有盲点，宋公常这么安(chao)慰(feng)我们。可很快，它们还是被我一一否决了！

具体我是这么做的：
对于硬件故障，easy，网线坏了换几根好的，网口坏了插几个好的，排除！网卡嘛...暂时咱不折腾了，还得把主机拆开，有点小麻烦，并且可能性微乎其微，讲道理一插上网线就可以**ifconfig**查到已自动分配好校园网IP或者**wireshark**抓到DHCP包的话，其实硬件基本就没毛病了。

网站无法连接，很大可能是域名解析出了问题，Ubuntu中两个与DNS相关的配置文件出错，测试发现最重要的一个配置文件——**/etc/resolv.conf**，“居然”会根据连接的网络的不同而动态变化：
{% codeblock %}
cat /etc/resolv.conf
{% endcodeblock %}

呐，连接到无线网OpenWrt的时候，是这样：
{% qnimg OS/Reinstallation/resolv-conf-wireless-openwrt.png %}

连接到有线网的时候，又变成介样：
{% qnimg OS/Reinstallation/resolv-conf-wired.png %}

网上的建议都是增加新的**nameserver**，比如全国通用DNS地址**114.114.114.114(114.114.115.115备用)**，还有全球通用的Google的DNS服务器IP：**8.8.8.8(8.8.4.4备用)**，OK，每个都试了下，都在这里不管用，可见问题并不出在这里。在后文总结部分再回来讨论这些配置文件。

至于软件故障，驱动程序首当其冲，google搜索相同系统和网卡，类似不能上网的故障还不少，解决方案也都还完备，一度来了信心，完全把“之前为啥是好的”抛诸脑后，试呗。

根据[**经验帖**](http://www.cnblogs.com/duwanjiang/p/5907634.html)，先确定当前以太网卡型号(注意**grep**命令对大小写敏感，搜关键字**ethernet**你是真的找不到网卡...)：
{% codeblock %}
sudo lspci | grep Ethernet
{% endcodeblock %}

网卡型号如下图所示，很普通的以太网卡：
{% qnimg OS/Reinstallation/ethernet-controller-type.png %}

进一步查看网卡所用驱动：
{% codeblock %}
sudo lshw -C network
{% endcodeblock %}

注意输出中的driver字段：
{% qnimg OS/Reinstallation/driver-8169.png %}

参考他人说法，更换驱动为r8168极有可能解决问题，于是去到官网找对应内核版本的[**驱动**](http://www.realtek.com.tw/downloads/downloadsView.aspx?Langid=1&PNid=13&PFid=5&Level=5&Conn=4&DownTypeID=3&GetDown=false#2)，发现提供Linux内核版本最新至4.7的驱动。
{% asset_link 0010-r8168-8.045.08.tar.bz2 r8168驱动下载 %}

解压后，进入驱动源码目录进行安装：
{% codeblock %}
cd r8168-8.045.08
sudo ./autorun.sh
{% endcodeblock %}

驱动安装完成后重启系统，重复上述命令，或者执行**lsmod**可以发现**r8168**驱动已经生效，然而，联网问题依然存在，也许是我与原博主发生的故障原因根本就不一样，自然不能适用。

驱动问题应该是可以排除了，痛定思痛，决定在线升级系统，兴许能解决网络模块故障：
{% codeblock %}
sudo apt-get update
sudo apt-get dist-upgrade
sudo reboot # 重启完成上述更新的安装
sudo update-manager -d # 打开更新管理器
{% endcodeblock %}

一连串更新操作之后，应该是这个界面：
{% qnimg OS/Reinstallation/update-manager.png %}

然后在某人的怂恿下...冒着软件配置在新系统Ubuntu 18(连官网都没有release...)上面各种不兼容报错的风险，我还是干了，其实这时候已经做好重装的准备了。

然后你还问然后？死心了！
一顿操作猛如虎，殊途同归重装苦/(ㄒoㄒ)/~~。

### 辩证地看问题
重装系统这个事儿吧，熟练了就是很简单的一件事儿，无非就是这么几步：
> (1) 找个**系统镜像**，如[**Ubuntu**](https://www.ubuntu.com/download/desktop)；
> (2) 做个**系统启动盘**，推荐[**UltraISO**](https://cn.ultraiso.net/)；
> (3) BIOS设置成从**U盘启动**，[**懒人教程**](https://jingyan.baidu.com/article/f0e83a25c2e28a22e5910134.html)送上；
> (4) U盘启动后根据对应操作系统装机提示按步操作。

以上，操作系统即安装完成，倒不算麻烦，顺利的话20多分钟(Ubuntu)应该能够搞定，日常使用的软件也都提前安装好了或者在应用商店基本都可以找到，基本的工作娱乐需求也基本满足了，但是，有一个问题，装Linux的人会仅仅满足于这些基本的软件功能嘛？——毕竟，以用户友好著称的Windows(仅限于Office...)显然才更应该是大多数人的首选。

(⊙v⊙)嗯...其实我想说的是，对于追求极致效率、捣鼓不倦的Coder们来说，重装系统的工作明明才刚刚开始好伐！

既然无可避免，何不带着享(xiang)受(shi)的心情，重新打造自己的虚拟空间？

## 重装进行时
---
俗话说：“年轻不怕重来，生命在于折腾”，那么，开始吧！
(以下系统配置与本人喜好有关，仅供参考)

### 目录结构
PC里面的文件目录将被设计成和浏览器中的书签一样都是精心整理过的，一是强迫症，二是方便找。
{% qnimg OS/Reinstallation/chrome-bookmark.png %}

主要在用户**home**目录下新建若干常用目录**Mine**、**Code**、**Software**。
安装**tree**命令
{% codeblock %}
sudo apt-get install tree
{% endcodeblock %}

查看目录树型结构如下：
{% qnimg OS/Reinstallation/dir-mine.png %}
{% qnimg OS/Reinstallation/dir-code.png %}
{% qnimg OS/Reinstallation/dir-software.png %}
{% asset_link dir.sh 目录配置脚本下载 %}

### 桌面主题
平时看厌了Ubuntu默认的玫红色桌面和终端，就想换个口味，这里推荐一款Ubuntu的扁平化桌面主题**Flatabulous**，加上扁平化的**icon**，显示效果是这样：
{% qnimg OS/Reinstallation/theme-flatabulou-desktop-terminal.png %}

按下win键触发搜索，界面是这样的：
{% qnimg OS/Reinstallation/theme-flatabulous-search.png %}

怎么样，喜不喜欢？只需三步你值得拥有：

第一步，安装**Unity Tweak Tool**：
{% codeblock %}
sudo apt-get install unity-tweak-tool
{% endcodeblock %}

win键搜索后**Unity Tweak Tool**打开后是这样，其中**theme**和**icon**是在第三步要配置的：
{% qnimg OS/Reinstallation/theme-unity-tweak-tool.png %}

第二步，配置ppa源安装**Flatabulous**主题和i**Flat Icon**图标：
{% codeblock %}
# theme
sudo add-apt-repository ppa:noobslab/themes
sudo apt-get update
sudo apt-get install flatabulous-theme
# icon
sudo add-apt-repository ppa:noobslab/icons
sudo apt-get update
sudo apt-get install ultra-flat-icons
{% endcodeblock %}

第三步，使用**Unity Tweak Tool**配置主题和图标：
打开**Unity Tweak Tool**，设置**theme**为**Flatabulou**，**icon**为**Ultra Flat**即可。

### 输入法
Ubuntu上面的中文输入法不是很好用，可以安装[**搜狗输入法**](https://pinyin.sogou.com/linux/?r=pinyin)进行替代，配置如下：
{% codeblock %}
sudo add-apt-repository ppa:fcitx-team/nightly
sudo apt-get update
sudo apt-get -y install fcitx
sudo apt-get -y install fcitx-config-gtk
sudo apt-get -y install fcitx-table-all
sudo apt-get -y install im-switch
{% endcodeblock %}

检查**fcitx**是否安装完成：win键搜索fcitx，再安装搜狗官网下载linux版本的deb包：
{% codeblock %}
sudo dpkg -i  文件名.deb
{% endcodeblock %}

在系统设置里面点击语言支持，将输入法从**ibus**改为**fcitx**：
{% qnimg OS/Reinstallation/sougou-ibus-to-fcitx.png %}

注销一次，找到fcitx的配置，添加**sougou pinyin**并设为第一输入法即可：
{% qnimg OS/Reinstallation/sougou-fcitx-add-sougou.png %}

配置完成即可使用，有细心的童鞋还会可能发现一个问题，那就是桌面右上角的任务栏会有两个输入法的图标，一个是搜狗的，另一个是fcitx，并且在打字时还会发现桌面同时出现两个输入框，觉得碍眼的朋友，请这么做，终端执行：
{% codeblock %}
ps aux | grep fcitx
{% endcodeblock %}

在输入中找到**fcitx-panel**那一行，然后将本行的第二个字段(pid，即进程号)对应数字记下，杀掉该进程即可：
{% codeblock %}
kill -9 进程号
{% endcodeblock %}

好了，多余的输入框和任务栏图标都没啦～

### 运行环境
下面是各种开发环境的配置，包括：
1. **zsh & oh-my-zsh**(fish也蛮不错的)
2. **git**
3. **vim & vimplus**
4. **gcc & g++**
5. **jdk**
6. **python2 & python3 & pip**
7. **nodejs & hexo**
8. **mysql & workbench**
9. **ssh**
10. **wireshark**
11. **Unix Network Programming**

配置详见[**此篇**](http://localhost:4000/2018/01/16/OS/Reinstallation/ubuntu-config-runtime/)

### 工作利器
顺便再安利几款Ubuntu下好用的利器：
1. **IDE**
**JetBrains**家的几个亲儿子就不用多说啦，选择合适量级的新版本装好：
（1）[**Java - Intellij**](https://www.jetbrains.com/idea/)
（2）[**C/C++ - Clion**](https://www.jetbrains.com/clion/)（vim大神求放过...个人觉得用IDE来管理工程模块代码还是会方便一些）
（3）[**Python - PyCharm**](https://www.jetbrains.com/pycharm/)
（4）[**Java Script - WebStorm**](https://www.jetbrains.com/webstorm/)
下载后解压，进入**bin**目录执行**x.sh**(x为IDE名称)的脚本即可打开，回头固定到**Launcher**上就好。
觉得哪用得上这么多的看过来，**Microsoft**家的[**VSCode**](https://code.visualstudio.com/)绝对能满足你，丰富的插件支持多种常见语言开发：C/C++、Java、Python等，文本编辑则有**MarkDown**和**LaTex**，配置简单，保证够用！
还嫌(re)弃(ai)麻(zhe)烦(teng)的，您出门右转那儿有个[**Sublimetext**](https://www.sublimetext.com/)，我就不送了...
Markdown编辑器的话，比较喜欢[**Remarkable**](https://remarkableapp.github.io/linux.html)的风格：
{% qnimg OS/Reinstallation/ide-markdown-remarkable.png %}
伸手党配置同一效果：**View -> Night Mode，Style -> Screen**

2. **虚拟化**
（1）[**Virtualbox**](https://www.virtualbox.org/wiki/Linux_Downloads)
（2）[**VMware**](https://my.vmware.com/cn/group/vmware/info/slug/desktop_end_user_computing/vmware_workstation_pro/14_0)
下载官方.deb包使用命令**sudo dpkg -i 文件名.deb**安装即可，如有因缺乏依赖导致安装失败时，根据命令行提示，执行：
{% codeblock %}
sudo apt-get -f install 
{% endcodeblock %}
一键安装之前缺乏的依赖，再次安装.deb包即可成功。

3. **阅读器**
推荐[**Okular**](https://apps.ubuntu.com/cat/applications/quantal/okular/)，系统自带应用商店**Ubuntu Software**就有：
{% qnimg OS/Reinstallation/okular.png %}
快捷键F6进入文档操作模式，再按数字键4，即可高亮文档，但是有个坑就是它高亮部分的文件状态信息大概是维护在软件内部的，也就是说根本不会对源文件做修改...这导致换个程序换个机器打开文档后都只能看到未经处理的源文档...

4. **截图**
推荐**Shutter**，安装命令如下：
{% codeblock %}
sudo apt-get install shutter
{% endcodeblock %}
为其配置系统快捷键**Ctrl+Alt+A**为调用**shutter**进行区域截图，快捷键Name随意起，Command填入**shutter -s**：
{% qnimg OS/Reinstallation/shutter.png %}

5. **音乐、翻译**
[**网易云**](https://music.163.com/#/download)Ubuntu 16.04版和[**有道**](http://cidian.youdao.com/index-linux.html)Deepin版，不多说，官网的.deb直接安装

6. **VPN**
使用**ShadowSocks**在Ubuntu下科学上网，浏览器和终端翻墙都花了不少功夫，详见[**此篇**](http://localhost:4000/2018/01/21/OS/Reinstallation/ubuntu-config-vpn/)

## 重装的收获
---
### 遵循基本规定
- **编译安装的文件一般安装在/usr/local/filename**
具体命令如下：
{% codeblock %}
cd 源码文件夹
./autogen.sh # 生成configure脚本
./configure --prefix=/usr/local/软件名
make -j4 # 多(四)线程编译
sudo make install # 安装
{% endcodeblock %}
以上命令会将此软件相关的文件都会”安装“到同一个文件夹**/usr/local/软件名**下去，这样做的一个显而易见的好处是：如果之后想要删除该文件，或安装更新版本时要求彻底卸载旧版本，只需删除此文件夹即可实现完全删除，再也不用担心由于卸载不干净导致影响新版本安装配置失败的问题。
然而这样做在方便软件管理的同时也带来了一个额外的操作：每次安装新命令时，需要配置**PATH**环境变量，将该命令的安装文件夹下的**bin**文件夹的绝对路径添加到原有**PATH**变量后方才能使用新安装软件命令：
{% codeblock %}
vim ~/.zshrc # 本人习惯将环境变量定义在此配置文件中
# 末尾增加一行 export PATH=$PATH:/usr/local/软件名/bin
source ~/.zshrc # 令新修改的配置文件生效
{% endcodeblock %}
再来理解一下软件“**安装**“的概念：其实就是把软件的编译好的**可执行文件**和**依赖库文件**放到一个系统能够发现的合适位置。那么放在什么路径下才能让系统自动找到呢？这就是环境变量的作用了，在Linux乃至Windows系统中，**PATH**无疑是最重要的一个系统环境变量，它是一个由”:“(Linux)或”;“(Windows)分隔的，由多个软件可执行文件所在路径连接所构成的字符串，在未显式指定可执行文件的情况下，在终端中执行命令，都会到由**PATH**变量中包含的文件路径下面去找该命令对应的可执行文件，如果没有，就会报”**Command Not  Found**“的错误。

### 善用系统命令
1. **sudo apt-get -f install** 
正确安装之前由于缺乏依赖而失败的安装过程中全部必要依赖，避免要对照着安装错误日志上面的依赖名称一个个安装。
2. **sudo add-apt-repository ppa:x** 
添加ppa软件源，加入-r参数则表示remove即删除某个之前添加的ppa软件源，回车两下后操作生效，对应的ppa源的增删情况可以在两个地方得到验证：
**（1）系统设置(System Settings) -> 软件更新(Software & Update) -> 其它软件(Other Software)**：列表中会显示/移除对应的ppa源地址，你当然可以在该UI上直接操作；
**（2）/etc/apt/source.list.d**：目录下会出现/删除对应ppa源的.list和.list.save文件。

3. **whereis、local和which**
（1）**whereis**
**whereis**命令只能用于程序名的搜索，而且只搜索**二进制文件**（参数-b）、**man说明文件**（参数-m）和**源代码文件**（参数-s）。如果省略参数，则返回所有信息。
和**find**相比，**whereis**查找的速度非常快，这是因为linux系统会将 系统内的所有文件都记录在一个数据库文件中，当使用**whereis**和下面即将介绍的**locate**时，会从数据库中查找数据，而不是像**find**命令那样，通过遍历硬盘来查找，效率自然会很高。 
但是该数据库文件并不是实时更新，**默认情况下时一星期更新一次**，因此，我们在用**whereis**和**locate** 查找文件时，有时会找到已经被删除的数据，或者刚刚建立文件，却无法查找到，原因就是因为数据库文件没有被更新。 
（2）**locate**
**locate**让使用者可以很快速的搜寻档案系统内是否有指定的档案。其方法是先建立一个包括系统内所有档案名称及路径的数据库，之后当寻找时就只需查询这个数据库，而不必实际深入档案系统之中了。在一般的发行版中，数据库的建立都被放在**crontab**中自动执行。
**locate**命令其实是"**find -name**"的另一种写法，但要比后者快得多，原因在于它不搜索具体目录，而是搜索一个数据库（Ubuntu下是/var/lib/mlocate/mlocate.db），这个数据库中含有本地所有文件信息。Linux系统自动创建这个数据库，并且每天自动更新一次，所以使用**locate**命令查不到最新变动过的文件。为了避免这种情况，可以在使用**locate**之前，先使用**updatedb**命令，手动更新数据库。
（3）**which**
**which**命令的作用是，在**PATH**变量指定的路径中，搜索某个系统命令的位置，并且返回第一个搜索结果。也就是说，使用**which**命令，就可以看到某个系统命令是否存在，以及执行的到底是哪一个位置的命令。

4. **echo >> 和echo >**
在**Shell**编程过程很多时候会使用**echo**并输入到日志文件中。写日志的时候有两种情况，一种是一次写入文件空，再写的时候就将之前的内容给覆盖掉，如何实现追加内容呢？
（1）**覆盖写入**
{% codeblock %}
echo "日志内容"  > 文件
{% endcodeblock %}
（2）**追加写入**
{% codeblock %}
echo "日志内容"  >> 文件
{% endcodeblock %}

5. **lspci和lsmod**
（1）**lspci**
**lspci**命令用于显示当前主机的所有PCI总线信息，以及所有已连接的PCI设备信息。
（2）**lsmod**
**lsmod**命令用于显示已经加载到内核中的模块的状态信息。执行**lsmod**命令后会列出所有已载入系统的模块。**Linux**内核具有模块化的特性，应此在编译内核时，务须把全部的功能都放入内核。可以将这些功能编译成一个个单独的模块，待需要时再分别载入。

6. **insmod、rmmod、modprobe、depmod**
（1）**insmod**
**insmod**命令用于将给定的模块加载到内核中。Linux有许多功能是通过模块的方式，在需要时才载入内核执行。如此可使内核较为精简，进而提高效率，以及保有较大的弹性。这类可载入的模块，通常是**设备驱动程序**。
（2）**rmmod**
**rmmod**命令用于从当前运行的内核中移除指定的内核模块。执行**rmmod**可删除不需要的模块。
（3）**modprobe**
**modprobe**命令用于智能地向内核中加载模块或者从内核中移除模块。
**modprobe**可载入指定的个别模块，或是载入一组相依的模块。**modprobe**会根据**depmod**所产生的相依关系，决定要载入哪些模块。若在载入过程中发生错误，在**modprobe**会卸载整组的模块。
（4）**depmod**
**depmod**命令可产生模块依赖的映射文件，在构建嵌入式系统时，需要由这个命令来生成相应的文件，由**modprobe**使用。

7. **查看操作系统及内核版本**
（1）系统版本
{% codeblock %}
cat /etc/issue
{% endcodeblock %}
直接显示Ubuntu 16.04.3 LTS
或：
{% codeblock %}
sudo lsb_release -a
{% endcodeblock %}
显示系统版本号(Release)：Ubuntu 16.04.3 LTS，及代号(Codename)：Xenial
（2）内核版本
{% codeblock %}
uname -r
{% endcodeblock %}
显示当前内核版本：4.13.0-26-generic

8. **查看CPU信息**
（1）按单个逻辑核粒度
{% codeblock %}
cat /proc/cpuinfo
{% endcodeblock %}
（2）按总核粒度
{% codeblock %}
lscpu
{% endcodeblock %}

### 认识配置文件
1. **环境变量配置文件：**
（1）**/etc/profile**：此文件为系统的**每个用户**设置环境信息。当用户登录时，该文件被执行一次，并从 **/etc/profile.d**目录的配置文件中搜集**shell**的设置。一般用于**设置所有用户使用的全局变量**；
（2）**~/.bashrc**：只对**单个用户**生效，当登录以及每次打开新的**bash shell**时，该文件被读取；
（3）**~/.zshrc**：只对**单个用户**生效，当登录以及每次打开新的**zsh shell**时，该文件被读取。
我的环境变量一般都配置在**~/.zshrc**中，保存文件后别忘了用**source**使之立即生效。

2. **sudo配置文件：**
**/etc/sudoers**：从源码编译安装[**mininet**](http://mininet.org/)时，执行安装脚本：**sudo ./install.sh**，怎么配置也找不到之前安装好的**git**和**vim**，我们都知道"Command Not Found"错误的原因就是没配置好对应命令的环境变量，于是我把上面提到的配置文件都加上了**GIT_HOME**和**VIM_HOME**，即使切换到**root**用户，仍然报错，于是猜想**sudo**命令虽然是让普通用户能够在执行命令时以**root**权限运行，但是仍不是真正的**root**，上述对所有用户都生效的环境变量都对**sudo**不适用，原因应该是**sudo**自己有特殊的环境变量定义。google到的结果证实了我这一想法，在**/etc**目录下有一个**sudo**命令的配置文件**sudoers**：
{% qnimg OS/Reinstallation/sudoers-secure-path.png %}
以**sudo**执行命令都会从**/etc/sudoers**这个配置文件中变量**secure_path**的值作为**sudo**的**PATH**环境变量，会去这个变量指定的路径下去找命令对应的可执行文件。将**GIT_HOME**和**VIM_HOME**的添加到**secure_path**末尾，命令就能顺利找到执行了。

3. **内核驱动文件：**
**/lib/modules/$(uname -r)/kernel/drivers/**：网卡驱动会放在**net**文件夹中，**ethernet**表示以太网卡，**wireless**中放的是无线网卡驱动。

4. **DNS配置文件：**
（1）**/etc/resolv.conf**
（2）**/etc/resolvconf/resolvconf.d/base**
（3）**/etc/resolvconf/resolvconf.d/head**
里面最关键的就是**nameserver**字段，在其**IP**所对应的**DNS Server**处**search**当前想要连接的**域名**，如**nju.edu.cn**或**lan**等，一般来讲在Ubuntu 16.04系统中，每次更换网络连接后，**resolv.conf**都会自动重新加载，**nameserver**一般为**127.0.1.1**，一个很奇怪的**IP**地址，我们知道本机的回环地址**lo**的**IP**为**127.0.0.1**。查找资料后才知道，原来Ubuntu下是有一个本地DNS服务，叫做**dnsmasq**，由NetworkManager控制，通过以下命令能发现此服务进程信息：
{% codeblock %}
ps aux | grep dnsmasq
# 以下命令效果同上
ps -ef | grep dnsmasq
{% endcodeblock %}
输出如下：
{% qnimg OS/Reinstallation/dnsmasq-listen-addr.png %}
可以看到它监听的本地地址，**--listen-address=127.0.1.1**(ubuntu12.04及之前的版本是 127.0.0.1)， 这个地址是一个本地回环地址，而你真实的dns服务器地址，是被这个服务管理维护着的。(之后我会写篇[**文章**](http://localhost:4000/2018/01/21/OS/Reinstallation/ubuntu-dns/)专门讨论**Ubuntu**下的**DNS**配置)
即，用户进程想要访问某个**IP**未知、只知道主机名的服务器时，其发起DNS请求的流程如下：
> local process -> local dnsmasq -> router -> ISP DNS

5. **软件源配置文件：**
（1）**/etc/apt/source.list**:通常放的是[**官方镜像源**](http://cn.archive.ubuntu.com/ubuntu/)，但是中间偶然一天遇到了官方源挂掉的情况，考虑到教育网的缘故，我就更换成了[**清华的源**](https://mirror.tuna.tsinghua.edu.cn/help/ubuntu/)；
（2）**/etc/apt/source.list.d**：通常放的是ppa等第三方镜像源。

### 掌握实用操作
好奇之人对新知识总是充满着渴望，这次掌握几个小**Trick**，提(ke)高(xue)效(tou)率(lan)再进一步！
（1）**vim中临时提升编辑者权限**
在Linux上工作的朋友很可能遇到过这样一种情况，当你用**Vim**编辑完一个文件时，运行**:wq**保存退出，突然蹦出一个错误：
{% qnimg OS/Reinstallation/vim-readonly.png %}

这表明文件是只读的，按照提示，加上!强制保存：:w!，结果又一个错误出现：
{% qnimg OS/Reinstallation/vim-cannot-open-file-for-writing.png %}

文件明明存在，为何提示无法打开？查看**Vim帮助文档**对**E212**错误的解释如下：
{% qnimg OS/Reinstallation/vim-help-e212.png %}

原来是可能没有权限造成的，此时你才想起，这个文件需要**root**权限才能编辑，而当前登陆的只是普通用户，在编辑之前你忘了使用**sudo**来启动**Vim**，所以才保存失败。于是为了防止修改丢失，你只好先把它保存为另外一个临时文件**temp-file-name**，然后退出**Vim**，再运行**sudo mv temp-file-name readonly-file-name**覆盖原文件。
但这样操作过于繁琐。而且如果只是想暂存此文件，还需要接着修改，则希望保留**Vim**的工作状态，比如编辑历史，**buffer**状态等等，该怎么办？能不能在不退出**Vim**的情况下获得**root**权限来保存这个文件？
答案是肯定的，执行这样一条命令即可(原理参见[**这篇**](http://feihu.me/blog/2014/vim-write-read-only-file/))：
{% codeblock %}
:w !sudo tee %
{% endcodeblock %}

（2）待更...

## 参考资料
---
[1][Ubuntu 16.04 RTL8111/8168/8411 不能上网 经常断网解决办法]((https://shipengliang.com/software-exp/ubuntu-16-04-rtl811181688411-%E4%B8%8D%E8%83%BD%E4%B8%8A%E7%BD%91-%E7%BB%8F%E5%B8%B8%E6%96%AD%E7%BD%91%E8%A7%A3%E5%86%B3%E5%8A%9E%E6%B3%95.html)
[2][Ubuntu系统升级](http://topspeedsnail.com/upgrade-to-ubuntu-16_04-LTS/)
[3][Ubuntu下超好看的主题Flatabulous](https://www.jianshu.com/p/463f229c0a20)
[4][Ubuntux新安装后的软件准备—秋波](http://www.cnblogs.com/burningTheStar/p/6978209.html)
[5][dpkg安装deb缺少依赖包的解决方法](http://blog.csdn.net/seek_of/article/details/77920639)
[6][Ubuntu截图软件shutter](http://www.cnblogs.com/EasonJim/p/7114679.html)
[7][在Ubuntu中添加和删除ppar软件源](http://blog.csdn.net/lu_embedded/article/details/55803500)
[8][Linux的五个查找命令-阮一峰](http://www.ruanyifeng.com/blog/2009/10/5_ways_to_search_for_files_using_the_terminal.html)
[9][每天一个Linux命令](http://www.cnblogs.com/peida/)
[10][echo写入文件](http://blog.csdn.net/jakemanse/article/details/8043603)
[11][ubuntu dns服务器127.0.1.1的问题](https://www.jianshu.com/p/8f36fe09e39e)




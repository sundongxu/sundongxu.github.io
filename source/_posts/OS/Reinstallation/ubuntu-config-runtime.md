---
title: Ubuntu 16.04 下一键配置编程开发环境
date: 2018-01-16 12:12:12
categories:
- Operating System
tags:
- Linux
- Ubuntu
- Git
- Zsh
- Vim
- C/C++
- Java
- Python
- Hexo
---

## 全文概要
本文详细介绍了 Ubuntu 16.04 系统安装之后的开发环境搭建过程，包括Shell(zsh/fish)、Git、Vim等必备工具，C/C++、Java与Python等编程语言SDK，以及本博客使用的 Hexo + GitHub 框架的配置全记录。
<!--more-->

## 安装方式
---
Ubuntu系统中，安装软件通常有如下三种方式：**apt-get**、**dpkg**、**源码编译安装**。
### apt-get
Ubuntu 软件源本质上是一个软件仓库，我们可以通过以下命令来从仓库中下载并安装软件：
{% codeblock %}
$ sudo apt-get install 软件包名 
{% endcodeblock %}


上面命令中提到的 **apt-get** 是 Ubuntu 系统中的一个包管理工具，通过该方式安装新软件的优缺点小结如下：
> **优点**：官方源或PPA下载安装的软件版本经过多次测试验证，与本系统兼容性最好，且安装方便，一条命令就能搞定；
> **缺点**：由于加入官方仓库的审核测试流程复杂而漫长，导致仓库中的软件版本通常都不是最新的，甚至落后好几个版本，无法使用其最新特性，在安装配置有特殊软件版本依赖的软件时，可能会出现版本不够的问题。

**apt-get** 安装文件的一些**默认路径**：
> **/var/cache/apt/archives**：下载的软件存放位置；
> **/usr/share**：安装后软件默认位置；
> **/usr/bin**：可执行文件位置；
> **/usr/lib**：库文件位置；
> **/etc**：配置文件位置。

现在你知道为啥 **apt-get install** 前面总要加 **sudo** 了 —— 安装过程会要求足够的权限以访问上述路径。

**apt-get** 其它几个常用命令如下：

{% codeblock %}
# 卸载软件
# ① 只卸载软件，不删除配置文件等软件相关文件：
$ sudo apt-get remove 软件包名
# ② 完全卸载软件及包括配置文件在内的所有相关文件：
$ sudo apt-get purge 软件包名
# 或
$ sudo apt-get remove 软件包名 --purge
# ③ 卸载软件并卸载相关依赖：
$ sudo apt-get autoremove 软件包名
# 升级已安装软件
$ sudo apt-get upgrade 软件包名
{% endcodeblock %}

apt-get是从软件仓库中获取并安装软件源的工具，[**前文**](http://dongdongdong.me/2018/01/15/OS/Reinstallation/ubuntu/)提到过，Ubuntu会从以下两处加载配置：
> **/etc/apt/source.list**
> **/etc/apt/source.list.d**

前者是**官方软件仓库源**列表，包含绝大多数Ubuntu系统软件，默认是采用的[**官方中国镜像源**](http://cn.archive.ubuntu.com/ubuntu/)，教育网推荐[**清华镜像源**](https://mirrors.tuna.tsinghua.edu.cn/ubuntu/)。此前，Ubuntu 还没有位于国内的镜像源，从 Ubuntu 官方源下载软件比较慢，所以通常需要更换软件源来加快下载速度。互联网上有很多开源镜像站点，具体见[**此处**](http://wiki.ubuntu.org.cn/%E6%A8%A1%E6%9D%BF:16.04source)。选择源列表的时候，可以先使用 ping 命令测试一下网络速度，选择最快的源。

后者是一个目录，存放的是记录各PPA源镜像地址的配置文件，执行以下命令可以修改PPA源：
{% codeblock %}
# 新增PPA源
$ sudo add-apt-repository ppa:用户名/软件包名
# 删除PPA源
$ sudo add-apt-repository -r ppa:用户名/软件包名
{% endcodeblock %}

PPA软件源即 **Personal Package Archives (个人软件包档案)**。有些软件没有被选入 UBuntu 官方软件仓库，但为了方便Ubuntu用户使用，[**Launchpad**](https://launchpad.net/) 提供了PPA，允许用户建立自己的软件仓库，自由的上传软件。PPA也被用来对一些打算进入Ubuntu官方仓库的软件，或者某些软件的新版本进行测试。

Launchpad 是 Ubuntu 母公司 Canonical 有限公司所架设的网站，是一个提供维护、支援或联络 Ubuntu 开发者的平台。

无论是修改了官方源还是PPA源，都需要执行以下命令以更新软件源列表，使修改生效：
{% codeblock %}
$ sudo apt-get update
{% endcodeblock %}

### dpkg
**deb** 是 **debian** 软件包安装格式，跟 **red hat** 的 **rpm** 非常相似，通常可以到软件产品的提供商的官方网站上找到 **deb** 安装包，最基本的安装命令是：
{% codeblock %}
$ dpkg -i 软件包名.deb
{% endcodeblock %}

此时若报缺乏特定依赖的错误，可以使用如下命令一键安装依赖：
{% codeblock %}
$ sudo apt-get -f install
{% endcodeblock %}

**dpkg** 是 **Debian Package** 的简写，是为 **Debian** 专门开发的套件管理系统，方便软件的安装、更新及移除。所有源自 **Debian** 的 **Linux** 发行版都使用 **dpkg**，例如 **Ubuntu**、**Knoppix** 等等。 

通过该方式安装新软件的优缺点小结如下：
> **优点**：在官方下载的指定系统的安装包后，版本最新，安装方便，一条命令就能搞定；
> **缺点**：经常会因缺乏依赖导致安装出错，但好在也只需一条命令即可安装全部所需依赖。

Ubuntu 中所有 **packages** 的信息都在 **/var/lib/dpkg/**目录下，其中子目录 **/var/lib/dpkg/info** 用于保存各个软件包的配置文件列表.不同后缀名代表不同类型的文件，如:
> **.conffiles**：记录软件包的配置文件列表
> **.list**：保存软件包中的文件列表，用户可以从.list的信息中找到软件包中文件的具体安装位置
> **.md5sums**：记录软件包的md5信息，用于包验证
> **.prerm**：脚本在Debian升级包之前运行，主要作用是停止作用于即将升级的软件包的服务,直到软件包安装或升级完成
> **.postinst**：脚本完成Debian包解开之后的配置工作，通常用于执行所安装软件包的相关命令和服务的重新启动
> **/var/lib/dpkg/available**：文件的内容是软件包的描述信息，该软件包括当前系统所使用的Debian安装源中的所有软件包,其中包括当前系统中已安装的和未安装的软件包

以下是一些 **dpkg** 的常见用法：

{% codeblock %}
$ dpkg -i 软件包名.deb # 安装一个 Debian 软件包，如你手动下载的文件
$ dpkg -r 软件包名 # 移除一个已安装的包
$ dpkg -c 软件包名.deb # 列出 package.deb 的内容
$ dpkg -I 软件包名.deb # 从 package.deb 中提取包信息
$ dpkg -P 软件包名 # 完全清除一个已安装的包，remove 仅删除数据和可执行文件，purge 另还删除所有相关的配置文件
$ dpkg -L 软件包名 # 列出包安装的所有文件清单
$ dpkg -s 软件包名 # 显示已安装的包的信息
$ dpkg-reconfigure 软件包名 # 重新配置一个已经安装的包
$ dpkg -S 软件包名 # 查看软件在哪个包里
{% endcodeblock %}

### 源码编译
源码来源：
> 官方源码压缩包
> 从GitHub等软件仓库中拉取

通过此方式安装软件的优缺点小结如下：
> **优点**：版本无疑最新，甚至还可以安装尚未正式释出的测试版，另还可灵活指定安装路径；
> **缺点**：编译、配置步骤较为繁琐，经常需要查看错误日志定位问题。

**编译安装的文件一般安装在/usr/local/filename**
具体命令如下：
{% codeblock %}
$ cd 源码文件夹
$ ./autogen.sh # 生成configure脚本
$ ./configure --prefix=/usr/local/软件名
$ make -j4 # 多(四)线程编译
$ sudo make install # 安装
{% endcodeblock %}

关于此步骤，我在之前一篇博文中有写到：
> 以上命令会将此软件相关的文件都会”安装“到同一个文件夹**/usr/local/软件名**下去，这样做的一个显而易见的好处是：如果之后想要删除该文件，或安装更新版本时要求彻底卸载旧版本，只需删除此文件夹即可实现完全删除，再也不用担心由于卸载不干净导致影响新版本安装配置失败的问题。
> 然而这样做在方便软件管理的同时也带来了一个额外的操作：每次安装新命令时，需要配置**PATH**环境变量，将该命令的安装文件夹下的**bin**文件夹的绝对路径添加到原有**PATH**变量后方才能使用新安装软件命令：
{% codeblock %}
$ vim ~/.zshrc # 本人习惯将环境变量定义在此配置文件中 
# 末尾增加一行 export PATH=$PATH:/usr/local/软件名/bin
$ source ~/.zshrc # 令新修改的配置文件生效
{% endcodeblock %}

作为开发人员，大多时候还是更倾向于编译安装，后文中我会根据软件实际选择最合适的安装方式，让我们开始吧～

## 必备工具
---
### Git
最方便的方式，当然是 **Apt** 安装：
{% codeblock %}
$ sudo apt-get install git
{% endcodeblock %}

缺点之前也说了，版本太老，截至到本文完成，在 Ubuntu 16.04 系统上通过该方式安装的 **Git** 版本才 **2.7.4**，而官方已经释出的版本到了 **2.16.1**，不多说，源码安装步骤如下：
{% codeblock %}
$ wget https://www.kernel.org/pub/software/scm/git/git-2.16.1.tar.gz # 拉取官方源码包
$ tar zxvf git-2.16.1.tar.gz
$ cd git-2.16.1
$ ./configure --prefix=/usr/local/git # 配置Git安装路径
$ make -j4 # 4线程编译
$ sudo make install #　安装Git到/usr/local/git中
$ git --version # 查看已安装版本
{% endcodeblock %}

### Zsh
#### Shell
Shell 是 Linux/Unix 的一个外壳，理解成衣服也行，它负责外界与Linux内核的交互，接收来自用户或其他应用程序的命令，然后把这些命令转化成**内核**能理解的语言，传给内核，内核是真正干活的，干完之后再把结果返回用户或应用程序。

Linux/Unix提供了很多种Shell，为啥要这么多Shell？那我问你，同类型的衣服你怎么要买那么多件？花色、质地、风格不一样呗。写程序比买衣服复杂多了，而程序员往往负责把复杂的事情搞简单，简单的事情搞复杂。大牛程序员看到不爽的Shell，就会自己重新写一套，慢慢形成了一些标准，常用的Shell有这么几种，sh、bash、csh等，想知道你的系统有几种shell，可以通过以下命令查看：
{% codeblock %}
$ cat /etc/shells
{% endcodeblock %}

由于我已经事先安装好了**Zsh**和**Fish**，所以看起来是这样的：
> /bin/sh
> /bin/dash
> /bin/bash
> /bin/rbash
> /bin/zsh
> /usr/bin/zsh
> /usr/bin/fish

目前常用的 Linux 系统和 MacOS 系统的默认 Shell 都是 bash，但是真正强大的 Shell 是深藏不露的 **zsh**， 这货绝对是飞机中的战斗机，但是之前由于配置过于复杂，所以初期无人问津，很多人跑过来看看 zsh 的配置指南，什么都不说转身就走了。直到有一天，国外有个穷极无聊的程序员开发出了一个能够让人快速上手的zsh项目，叫做[**oh my zsh**](http://ohmyz.sh/)，GitHub地址在[**这里**](https://github.com/robbyrussell/oh-my-zsh)，这玩意差不多就像是《XX天从入门到精通》系列，宣传让人神功速成，而且居然是真的...

#### 安装
**Zsh** 安装也很方便：
{% codeblock %}
$ sudo apt-get install zsh git wget
$ wget --no-check-certificate https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh # 下载并安装Zsh
$ chsh -s /bin/zsh # 修改默认Shell为Zsh
# 注销后生效
{% endcodeblock %}

安装 **oh my zsh** 可以自动安装也可以手动安装
{% codeblock %}
# 自动安装：
$ wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh
# 手动安装
$ git clone git://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh
$ cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc
{% endcodeblock %}

#### 配置
**zsh** 的配置主要集中在用户当前目录的 **.zshrc** 里，可以在此处定义自己的环境变量和别名，当然，**oh my zsh** 在安装时已经自动读取当前的环境变量并进行了设置，你可以继续追加其他环境变量：
{% qnimg OS/Config-Runtime/zsh-environmental-variables.png %}

接下来进行别名的设置，部分常用别名设置如下：
{% qnimg OS/Config-Runtime/zsh-alias.png %}

设置完环境变量和别名之后，**Zsh** 基本上就很好用了。如果你是个主题控，还可以玩玩 **Zsh** 的主题。在 .zshrc 里找到ZSH_THEME，就可以设置主题了，默认主题是：
> ZSH_THEME="robbyrussell"

**oh my zsh** 提供了数十种主题，相关文件在 **~/.oh-my-zsh/themes** 目录下，默认提供了100多种，你可以随意选择，主题效果可在[**这里**]预览，也可以编辑主题满足自己的变态需求，我就用的默认主题 **robbyrussell**，大致效果如下：
{% qnimg OS/Config-Runtime/zsh-robbyrussell.png %}

#### 插件
oh my zsh 项目提供了完善的插件体系，相关文件在 **~/.oh-my-zsh/plugins** 目录下，默认提供了200多种，大家可以根据自己的实际学习和工作环境采用，各插件功能可在[**这里**](https://github.com/robbyrussell/oh-my-zsh/wiki/Plugins-Overview)进行简单了解，而具体细节则需打开相关目录下的 **.zsh** 文件查看。插件也是在 **.zshrc** 里配置，找到 **plugins** 关键字，你就可以加载自己的插件了，系统默认加载 **git** ，你可以在后面追加内容，我暂时启用了以下4个插件：
> plugins=(git autojump z extract)

上述插件功能说明如下：
（1）**git**
    当你处于一个 git 受控的目录下时，Shell 会明确显示 git 和 branch，如下图所示，另外对 git 很多命令进行了简化：

<style>
  table th:nth-of-type(1){
      width:150px;
  }
</style>

<style>
  table th:nth-of-type(2){
      width:150px;
  }
</style>

|   Alias   |      Command      |               Function             |
| :-------- | :---------------- | :--------------------------------- |
|   g       | git               |  命令原型     |
|   ga      | git add           |  将本地修改保存至暂存区以备提交         |
|   gb      | git branch        |  查看本地分支                        |
|   gcam    | git commit -a -m  |  提交暂存区修改，后接自定义提交日志信息  |
|   gd      | git diff          |  查看当前分支相比上次提交的版本的修改情况 |
|   gl      | git pull          |  从远端仓库拉取更新                   |
|   gp      | git push          |  将本地分支推送到远端仓库              |
|   gco     | git checkout      |  切换当前分支                        |
|   gm      | git merge         |  合并远端分支与本地分支                |
|   gst     | git status        |  查看本地分支情况(未暂存、已暂存未提交)  |

熟练使用可以大大减少 git 的命令长度，更多内容可以参考本地文件：
> **~/.oh-my-zsh/plugins/git/git.plugin.zsh**

（2）**extract**
    一个功能强大的解压插件，所有类型的文件解压只用一个命令 **x** 全搞定，再也不需要去记tar后面到底是哪几个参数了(上面的.zshrc中的别名配置中有提到不同类型的归档文件的解压命令和参数各不相同)。

（3）**autojump**与**z**：，
经常在命令行下工作的人应该都会遇到过这种情况，需要在几个目录直接来回跳转，不停的 **cd**，效率很低，有的时候还容易进入错的目录。虽然有 **pushd** 和 **popd** 这样的命令存在，但是还是不能做到随心所欲的跳转，于是一些目录跳转工具就被发明出来了。

它们的基本原理是，在每次 **cd** 的时候记录当前的路径，将这些路径按照 **cd** 进入的次数排序，也就是学习你经常使用的目录。一段时间之后，基本上可以通过前几个字母就能区分出你想进入的目录了，然后就可以输入前几个字母直接进行跳转，而不需要各种 **cd**。

这类工具中最早的应该是大名鼎鼎的 [**autojump**](https://linux.cn/article-3401-1.html)，GitHub项目地址在[**这里**](https://github.com/wting/autojump)，快捷命令是 **j**。**autojump** 使用Python编写，对于Bash和Shell的支持都比较好。但是可能是因为是Python写的吧，有的时候会感觉反应有些慢。

有了 **j** 之后，又有了 **z**。**z** 的介绍就是"更好的 **j**"。它的功能和 **j** 基本是相同的，不过它使用Shell脚本编写，更加简洁，基本不会拖慢终端的响应速度。我比较喜欢简洁的，现在看来Github上大部分人也是, **j** 得到了3000+的star，超越了它的前辈 **autojump**。进一步了解请参见[**这篇**](http://www.zcfy.cc/article/become-a-command-line-power-user-with-oh-my-zsh-and-z)，详细用法参见 **z** 的[**Github项目地址**](https://github.com/rupa/z)。

然而人们还不满足，于是又有了大杀器 **Fasd**，GitHub项目地址在[**这里**](https://github.com/clvv/fasd)。**Fasd** 不光会记录目录，还会记录文件，也就是说它可以做到快捷打开某个深层目录的文件。**Fasd** 还可以通过配置，实现更加高级的功能。**Fasd** 与 **Zsh** 的结合也非常好，可以使用Tab键灵活的在几个目录中选择。可能是由于 **Fasd** 太强大了，虽然它使用Shell脚本写的，但是在使用的时候还是会感觉拖慢了终端的速度，特别是在执行 **ls -l** 的时候，会感觉输出明显慢了一拍。

三个工具各有各的特点，人们在追求命令行工作的效率上真的是永无止境的。如果有新的、更好的工具出现，欢迎留言告诉我，感谢赐教。

最后说一句，虽然 **oh-my-zsh** 提供了很多插件，不过也不要贪多哦，大量的插件会拖慢打开的速度，只安装常用的就好了。

### Vim
**Vim** 不用多说，熟练了就是Linux上最好的文本代码编辑器，强大的插件支持几乎能满足所有使用需求。接触 **Vim** 到现在也有几年了，但是之前配置 **Vim** 都是在网上找别人写好的配置文件，比如 **.vimrc**。但是别人配置的始终都不能够满足自己的需求，比如：强大的 C/C++ 代码提示补全功能、头文件/源文件切换、静态代码分析等功能，好容易找到 **VimPlus**，作者自己归纳了一些vim的插件，然后做成了一键安装程序，给广大 **Vimers**带来了福音。

#### Vim安装
**VimPlus** 安装脚本中已经默认会帮你重新拉取GitHub上最新的 **Vim** 源码并自动安装，其中 Ubuntu 16.04 系统安装 **Vim** 步骤如下：
（1）安装 **Vim** 相关库依赖
{% codeblock %}
$ sudo apt-get install -y ctags build-essential cmake python-dev python3-dev fontconfig
$ sudo apt-get install -y libncurses5-dev libgnome2-dev libgnomeui-dev libgtk2.0-dev libatk1.0-dev libbonoboui2-dev libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev python3-dev ruby-dev lua5.1 lua5.1-dev 
{% endcodeblock %}

（2）卸载 **Vim** 相关软件
{% codeblock %}
$ sudo apt-get -y remove vim vim-runtime gvim # 卸载系统自带或通过 apt-get 安装的 Vim
$ sudo apt-get -y remove vim-tiny vim-common vim-gui-common vim-nox
{% endcodeblock %}

（3）删除 **Vim** 相关文件夹
{% codeblock %}
$ sudo rm -rf ~/vim
$ sudo rm -rf /usr/share/vim/vim74
$ sudo rm -rf /usr/share/vim/vim80
{% endcodeblock %}

（4）拉取 **Vim** GitHub源码并编译安装
{% codeblock %}
$ git clone https://github.com/vim/vim.git ~/vim
$ cd ~/vim
$ ./configure --with-feature-huge --enable-multibyte --enable-rubyinterp --enable-pythoninterp --with-python-config-dir=/usr/lib/python2.7/config-x86_64-linux-gnu
 --enable-perlinterp --enable-luainterp --enable-gui=gtk2 --enable-cscope --prefix=/usr
$ make VIMRUNTIMEDIR=/usr/share/vim/vim80
$ sudo make install 
$ cd - 
{% endcodeblock %}

其中 **configure** 脚本执行参数说明如下：
> --with-features=huge：支持最大特性
--enable-rubyinterp：启用Vim对ruby编写的插件的支持
--enable-pythoninterp：启用Vim对python编写的插件的支持
--enable-luainterp：启用Vim对lua编写的插件的支持
--enable-perlinterp：启用Vim对perl编写的插件的支持
--enable-multibyte：多字节支持，可以在Vim中输入中文
--enable-cscope：Vim对cscope支持
--enable-gui=gtk2：gtk2支持，也可以使用gnome，表示生成gvim
--with-python-config-dir=/usr/lib/python2.7/config-i386-linux-gnu/：指定 **python** 路径，注意这里要修改成本机的　**python** 相关路径
--prefix=/usr：编译安装路径，也可以自己指定

#### VimPlus在线安装
以上，**Vim** 安装完成，接下来是 **VimPlus** 插件的安装，根据 **install.sh** 安装脚本，**VimPlus** 使用 [**Vundle**](https://github.com/VundleVim/Vundle.vim) 来管理 **Vim** 插件。

**Vim** 有三种类型的插件：
（1）Github上 vim-scripts 仓库的插件；
（2）Github上非 vim-scripts 仓库的插件；
（3）不在Github上的插件。

而对于不同的插件，**vundle** 自动管理和下载插件的时候，有不同的地址填写方法，有如下三类： 
（1）在Github上vim-scripts用户下的仓库,只需要写出repos（仓库）名称； 
（2）在Github其他用户下的repos, 需要写出”用户名/repos名”；
（3）不在Github上的插件，需要写出git全路径。

之后想要为 **Vim** 添加新插件，只需编辑 **.vimrc** 文件，在下图对应位置处添加插件的下载地址路径：
{% qnimg OS/Config-Runtime/vimplus-vimrc-vundle.png %}

保存 **.vimrc** 文件后，有以下两种方式开始下载安装插件：
（1）运行 vim ,再运行 :PluginInstall：
{% codeblock %}
$ vim
: PluginInstall
{% endcodeblock %}

（2）命令行执行或编辑以下内容到脚本中执行：
{% codeblock %}
$ vim -c "PluginInstall" -c "q" -c "q"
# 或
$ vim -c "PluginInstall" -c "qall" 
{% endcodeblock %}

开始安装插件时应该是这个样子：
{% qnimg OS/Config-Runtime/vimplus-plugins.png %}

其中要提一下的是 [**YouCompleteMe**](http://valloric.github.io/YouCompleteMe/) 这个插件，用于 **Vim** 自动补全，可由于某种不可描述的缘故，下载安装它非常慢。着急的话，可以采用下一节介绍的离线安装方法。

#### VimPlus离线安装
将 **vimrc** 中 **YouCompleteMe**那一行注释掉，之后再从下面的地址下载后进行离线安装：
{% asset_link YouCompleteMe.tar.gz YouCompleteMe下载 %}

{% codeblock %}
$ cd ~/Downloads # 浏览器下载后默认放在此路径下
$ mv YouCompleteMe.tar.gz ~/.vim/bundle
$ cd ~/.vim/bundle
$ tar zxvf YouCompleteMe.tar.gz
$ cd YouCompleteMe
$ ./install.py --clang-completer
{% endcodeblock %}

到这一步，安装已经完成，你会发现~目录有两个文件，一个是vim的配置文件 **.vimrc**，一个是 **YouCompleteMe** 的配置文件 **.ycm_extra_conf.py**，一般来说新建一个 main.cpp 来写C、C++程序来说是没有问题的，都会有语法补全，当你需要写一些项目并涉及到第三方库时，就需要修改 **.ycm_extra_conf.py** 了，具体步骤如下：
（1）将 **.ycm_extra_conf.py** 拷贝的项目的根目录。
（2）更改 **.ycm_extra_conf.py** 里面的 **flags**变量，添加 **第三方库** 路径和**工程子目录** 路径。

桌面版linux使用 [**vim-devicons**](https://github.com/ryanoasis/vim-devicons) 插件会出现乱码，需要设置终端字体为 **Droid Sans Mono for Powerline Nerd Font Complete** ，使用xshell等工具连接服务器linux的用户就没有必要使用 **vim-devicons** 了，可以在插件目录将 **vim-devicons** 目录删除，不然会导致 **NerdTree** 的缩进有问题。

**vim** 的插件需要设置好了快捷键才会发挥它的威力，有些插件的快捷键可以查看各自官网，下面罗列部分插件的快捷键：
- 显示目录树 <F3>
- 显示函数、变量、宏定义等 <F4>
- 显示静态代码分析结果 <F5>
- .h .cpp 文件快速切换 <F2>
- 转到声明 <, + u>
- 转到定义 <, + i>
- 打开include文件 <, + o>
- Buffer切换 <Ctrl + P/Ctrl + N>
- 光标位置切换 <Ctrl + O/Ctrl + I>
- 模糊搜索文件 <Ctrl + f>
- 注释 <gcc/gcap/gc/,ca/,cA>
- DirDiff :DirDiff <dir1> <dir2>
- 重复 .
- 改变主题 <F10/F9>

## 编程语言
---
### C/C++


### Java


### Python


## 博客框架
---


## 参考资料
---
[1][Ubuntu的使用-apt-get与dpkg](http://blog.csdn.net/lanchunhui/article/details/51446359)
[2][Ubuntu软件源](https://github.com/nodejh/nodejh.github.io/issues/26)
[3][终极Shell-MacTalk](http://macshuo.com/?p=676)
[4][j,z和fasd——Shell目录跳转工具](https://skyline75489.github.io/post/2014-12-13_j-z-and-fasd.html)
[5][Ubuntu下安装 Oh-My-Zsh 和 autojump](http://notes.11ten.net/zsh.html)
[6][通过 Oh-My-Zsh 和 Z 成为一个命令行高手](http://www.zcfy.cc/article/become-a-command-line-power-user-with-oh-my-zsh-and-z)
[7][超级强大的vim配置-vimplus](http://www.cnblogs.com/highway-9/p/5414465.html)
[8][超级强大的vim配置-vimplus(续集)](http://www.cnblogs.com/highway-9/p/5984285.html)
[9][打造一流编辑器vimplus](http://www.cnblogs.com/burningTheStar/p/6961220.html)
[10][在Ubuntu中安装Python的几种方法](http://blog.csdn.net/huanhuanq1209/article/details/72673236)
[11][从源码安装 Python3.6.3 后lsb_release失效-Stack Overflow](https://stackoverflow.com/questions/46752279/lsb-release-not-working-after-install-python-3-6-3-from-source)
[12][在Ubuntu 14.04 上升级Python后 Apt 出错-No Module named 'lsb_release'](https://unix.stackexchange.com/questions/174933/apt-broken-after-upgrading-python-on-14-04-no-module-named-lsb-release/174983)
[13][]()

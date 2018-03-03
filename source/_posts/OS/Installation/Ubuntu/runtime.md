---
title: Ubuntu 16.04 下一键配置编程开发环境
date: 2018-01-16 12:12:12
categories:
- Operating System
tags:
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
本文详细介绍了 Ubuntu 16.04 系统安装之后的开发环境搭建过程，包括Shell(zsh/fish)、Git、Vim等必备工具，C/C++、Java与Python等编程语言SDK的安装与配置方法。
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
参见[**这篇**](http://dongdongdong.me/2018/01/17/OS/Installation/Ubuntu/zsh/)

### Vim
参见[**这篇**](http://dongdongdong.me/2018/01/22/OS/Installation/Ubuntu/vim/)

## 编程语言
---
### C/C++
在Ubuntu下搭建C/C++编程环境，综合起来就是：**Vim + GCC + GDB**，其中 **Vim** 用于文本编辑，**GCC** 用于程序编译，**GDB** 用于代码调试。

其实刚装好的系统中已经有 **GCC** 了，但是这个 **GCC** 还无法编译文件，所以要安装 **build-essential**，作用是提供编译程序必须软件包的列表信息，也就是说编译程序有了这个软件包它才知道头文件在哪、库函数在哪，还会下载依赖的软件包，最后才组成一个开发环境。

安装编程环境，在终端中执行：
{% codeblock %}
$ sudo apt-get install build-essential
{% endcodeblock %}

#### GCC
**GCC (GNU Compiler Collection)**是一组编译工具的总称，支持多平台、多语言源文件到可执行文件的编译与生成。其中也包括 **gcc(C编译器)** 和 **g++(C++编译器)**。

在 **GCC** 内部寻找帮助，使用 **gcc --help**，如果想看 **gcc** 选项的完整列表使用 **gcc -v --help 2>&1 | more**。

**GCC** 基本语法格式如下：
{% codeblock %}
# 对于.c文件
$ gcc [options] [filenames]
# 对于.cpp或.cc
$ g++ [options] [filenames]
{% endcodeblock %}

命令选项参数及说明如下表：

<style>
  table th:nth-of-type(1){
      width:100px;
  }
</style>

|        Option       |                           Description                           |
|:--------------------|:----------------------------------------------------------------|
| -c	              | 只对文件进行编译和汇编，但不进行连接，生成目标文件”.o”                  |
| -S	              | 只对文件进行编译，但不汇编和连接                                     |
| -E	              | 只对文件进行预处理，但不编译汇编和连接                                |
| -g	              | 在可执行程序中包含标准调试信息                                       |
| -o file1 [file2]	  | 将文件file1编译成可执行文件file2                                    |
| -v	              | 打印出编译器内部编译各过程的命令行信息和编译器的版本                     |
| -I dir	          | 在头文件的搜索路径列表中添加dir目录                                   |
| -L dir	          | 在库文件的搜索路径列表中添加dir目录                                   |
| -static	          | 强制链接静态库                                                     |
| -lNAME	          | 连接名为libNAME的库文件                                            |
| -Wall -W	          | 开启GCC最常用的警告，GCC的warning一般格式为:file:line-number:message  |
| -pedantic	          | 要求严格符合ANSI标准                                               |
| -Wconversion	      | 开启隐式类型转换警告                                                |
| -Wshadow	          | 开启同名变量函数警告                                                |
| -Wcast-qual	      | 开启对特性移除的cast的警告，如const                                  |
| -o(-o1)	          | 对编译出的代码进行优化                                              |
| -o2	              | 进行比-o高一级的优化                                                |
| -o3	              | 产生更高级别的优化                                                  |
| -os	              | 产生最小的可执行文件                                                |
| -pg	              | 开启性能测试，记录每个函数的调用次数与时长                              |
| -ftest-coverage	  | 记录每一行被执行的次数                                              |
| -fprofile-arcs	  | 记录每个分支语句执行的频率                                           |

#### GDB
**GDB** 是一个用来调试 **C/C++** 程序的功能强大的调试器，能在程序运行时观察程序的内部结构和内存使用情况,它主要提供以下功能：
- 监视程序中变量的值的变化
- 设置断点，使程序在指定的代码行上暂停执行，便于观察
- 单步执行代码
- 分析崩溃程序产生的core文件

通过在 **GDB** 下输入 **help** 或在命令行上输入 **gdb h** 查看关于 **gdb** 选项说明的简单列表。键入 **help** 后跟命令的分类名。可以获得该类命令的详细清单。搜索和 **word** 相关的命令可用 **apropos word**。

为使 **GDB** 能正常工作，必须在程序编译时包含调试信息。即 **-g** 选项。前文有讲解。

简单的调试步骤示例：
1. 载入test可执行文件：gdb test --silent
2. 运行：run/r
3. 查看程序出错的地方：where
4. 查看出错函数附近的代码：list/l
5. 打开堆栈：backtrace/bt
6. 单步调试：next/n或step
7. 查看可疑表达式值：print var
8. 在可疑行打断点：break linenum
9. 重新运行会在断点处停止。用 set variable 修改变量值
10. 继续运行：continue，看结果是否正确
11. 退出gdb：quit

再放一张 **GDB** 命令示意图：
{% qnimg OS/Installation/Ubuntu/runtime/gdb-commands-list.png %}

### Java
高级编程语言都会有自己专门的开发环境，即 **Software Development Kit**，简称 **SDK**，对于 **Java** 而言，最常用的就是 **Oracle** 公司提供的 **[JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)**，全称是 **Java SE Development Kit**，现在官网最新版本是 **jdk-8u161**。

下载最新版本的 **JDK** 准备之后安装，如果系统预先安装好了 **OpenJDK** ，需要先将其卸载，否则可能出现冲突：
{% codeblock %}
$ sudo apt-get purge openjdk/openjdk*
$ sudo apt-get clean/autoclean
{% endcodeblock %}

解压 **JDK** ，并移动到指定位置：
{% codeblock %}
$ tar zxvf jdk-8u161-linux-x64.tar.gz
$ sudo mv ./jdk1.8.0_161 /usr/local/java
{% endcodeblock %}

配置环境变量：
{% codeblock %}
$ vim ~/.zshrc
# 将以下内容添加在文件末尾
# export Java_HOME=/usr/local/java/jdk1.8.0_161
# export JRE_HOME=$JAVA_HOME/jre
# export CLASSPATH=.:$CLASSPATH:$JAVA_HOME/lib:$JRE_HOME/lib
# export PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin
# 保存并退出 :wq
$ source ~/.zshrc # 使配置生效
{% endcodeblock %}

验证安装版本：
{% codeblock %}
$ java -version  # 显示：java version "1.8.0_161"
$ javac -version # 显示：javac 1.8.0_161 
{% endcodeblock %}

### Python
Ubuntu系统自带 **Python** 环境，**Python 2** 和 **Python 3** 都有，可以通过以下命令检查版本：
{% codeblock %}
$ python -V
$ python3 -V
{% endcodeblock %}

官网截止到本文发表时的最新 **Python** 版本分别为 **2.7.14** 和 **3.6.4**，下载两个版本 **.xz** 的 **Python** 源码并进行编译安装，步骤类似，以 **Python 2.7.14** 为例：
{% codeblock %}
# Ubuntu中先要安装ssl相关依赖
$ sudo apt-get install python-dev libffi-dev libssl-dev
$ wget https://www.python.org/ftp/python/2.7.14/Python-2.7.14.tar.xz
$ xz -d Python-2.7.14.tar.xz
$ tar xvf Python-2.7.14.tar
$ cd Python-2.7.14
$ ./configure
$ make -j
$ sudo make install
{% endcodeblock %}

执行 **configure** 脚本时可以通过 **prefix** 参数指定安装路径，默认会将可执行文件放至 **/usr/local/bin** 下。

#### pip
**Python** 语言的优势主要体现在其拥有功能强大的 **库** ，或者说是 **依赖模块**，例如：**Numpy** 适用于科学计算领域，**Pandas** 则带来强大的矩阵计算能力，**Matplotlib**、**Seaborn** 提供了样式繁多的绘图模板使得 **Python** 成为数据可视化领域一个有力工具，HTTP方面也有 **urllib3**、**requests** 等，而 **Flask** 更是一个性能优秀、功能完备的网络框架。

为加快项目产品迭代进度，不去重复造轮子，**Python** 编程的过程中不可避免地需要使用诸多成熟库，对于这些依赖库的管理(安装、升级、卸载等)一般使用 **pip** 包管理器，基本全部的常用库的稳定版本都可以直接从 **pip**
的软件源中下载，常用命令如下：
{% codeblock %}
$ sudo -H pip install package_name # 安装
$ sudo -H pip upgrade package_name # 升级
$ sudo -H pip uninstall package_name # 卸载
$ sudo -H pip list # 列举已安装的包
{% endcodeblock %}

源码安装的 **Python 3** 中会将最新版本的 **pip** 一并安装，通过 **pip** 安装的库会被安装至 **/usr/local/lib/python3.6/site-packages**。而 **python 2** 在编译安装时则不会安装 **pip** ，可通过以下方式安装：
{% codeblock %}
$ wget https://bootstrap.pypa.io/get-pip.py
$ python2 get-pip.py
{% endcodeblock %}

和 **Python** 分为 **python2** 和 **python3** 一样，**pip** 同样分为 **pip2** 和 **pip3** ，且分别为当前安装的 **python2** 和 **python3** 服务，即 **pip2** 安装的包只供 **python2** 环境导入使用的，**pip3** 安装的包只供 **python3** 环境导入使用。

## 博客框架
---
参见[**此篇**](http://dongdongdong.me/2018/01/25/OS/Installation/Ubuntu/blog-github-hexo.md/)

## 参考资料
---
[1][Ubuntu的使用-apt-get与dpkg](http://blog.csdn.net/lanchunhui/article/details/51446359)
[2][Ubuntu软件源](https://github.com/nodejh/nodejh.github.io/issues/26)
[3][build-essential的作用](http://www.cnblogs.com/bing-yu12/p/6384447.html)
[4][在Ubuntu下搭建C/C++编程环境](http://conglang.github.io/2015/03/17/programming-in-ubuntu/)
[5][Ubuntu 16.04 安装 JDK](https://www.jianshu.com/p/8712cf20a5c9)
[6][在Ubuntu中安装Python的几种方法](http://blog.csdn.net/huanhuanq1209/article/details/72673236)
[7][从源码安装 Python3.6.3 后lsb_release失效-Stack Overflow](https://stackoverflow.com/questions/46752279/lsb-release-not-working-after-install-python-3-6-3-from-source)
[8][pip 缺少 TLS/SSL Module](https://www.jianshu.com/p/553f9237576c)
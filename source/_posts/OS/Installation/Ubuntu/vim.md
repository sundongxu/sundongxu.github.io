---
title: Ubuntu 16.04 最好用的 Vim 配置
date: 2018-01-22 22:22:22
categories:
- Operating System
tags:
- Ubuntu
- Vim
- VimPlus
---

## 全文概要
---
关于**Vim** ，不消多说，熟练了就是Linux上最好的文本代码编辑器，丰富的命令和强大的插件支持几乎能满足即使最挑剔的那一部分人的需求。本文详细介绍了安装 **Vim** 的正确姿势以及一款超强大的 **Vim** 插件 **VimPlus** 的配置过程，更多的 **Vim** 使用技巧会在后续的博文随本文学习总结的进度同步推出。
<!--more-->

> VIM is the God of editors, EMACS is God’s editor.
EMACS is actually an OS which pretends to be an editor.

以上，大概是你能听到的关于编辑器的最高评价了。

回到本文的主角 **Vim** ，接触它到现在也有几年了，复杂繁多的命令着实让人经历了相当痛苦的一段磨合期。然而，本文志不在普及 **Vim** 的使用技巧，而是从其安装过程入手，教会你轻松配置一个更友好的 **Vim** 工作环境。

之前配置 **Vim** 都是在网上找别人写好的配置文件，比如 **.vimrc**。但是别人配置的始终都不能够满足自己的需求，比如：强大的 C/C++ 代码提示补全功能、头文件/源文件切换、静态代码分析等功能，直到 **VimPlus**，[**作者**](http://www.cnblogs.com/highway-9/)自己归纳了一些vim的插件，然后做成了一键安装程序，给广大 **Vimers**带来了福音。

## Vim安装
---
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

## VimPlus安装
---
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
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vimplus-vimrc-vundle.png)

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
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vimplus-plugins.png)

其中要提一下的是 [**YouCompleteMe**](http://valloric.github.io/YouCompleteMe/) 这个插件，用于 **Vim** 自动补全，可由于某种不可描述的缘故，下载安装它非常慢。着急的话，可以采用下面介绍的离线安装方法：
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

## VimPlus快捷键
---
**vim** 的插件需要设置好了快捷键才会发挥它的威力，有些插件的快捷键可以查看各自官网，下面罗列部分插件的快捷键：
> （1）显示目录树：F3
（2）显示函数、变量、宏定义等：F4
（3）显示静态代码分析结果(类型编译器检查语法错误多少个error多少个warning)：F5
（4）.h .cpp 文件快速切换：F2
（5）转到声明：, + u
（6）转到定义：, + i
（7）打开include文件：, + o
（8）Buffer切换：Ctrl + P 或 Ctrl + N
（9）光标位置切换：Ctrl + O 或 Ctrl + I
（10）模糊搜索文件：Ctrl + f
（11）注释：gcc/gcap/gc/,ca/,cA
（12）DirDiff：DirDiff <dir1> <dir2>
（13）重复 .
（14）前后更换Vim界面主题：F9与F10

## Vim教程
---
入门教程就放 [**简明 Vim 练级教程 - 酷壳**](https://coolshell.cn/articles/5426.html)

这里放几张 **Vim** 使用技巧的示意图，便于学习掌握：

先来张 **Vi/Vim** 键盘图： 
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vi-vim-cheat-sheet.png)

英文看不懂？没关系，下面是中文版：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vi-vim-cheat-sheet-cn.png)

程序员或许更喜欢这样的：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vim-cheat-sheet-for-programmers-screen.png)

还有这样的：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/vim/vim-cheat-sheet-en-visual.png)

总而言之，**Vim** 是一款功能超乎想象地强大齐全的编辑器。然而，使用者不可能会用到其所有的特性，我们只需掌握基本常用的操作命令，其余的在要使用时再去查阅教程或者攻略即可。

## 参考资料
---
[1][跟我一起学Vim—The life-changing Editor](http://feihu.me/blog/2014/intro-to-vim/#%E4%B8%BA%E4%BB%80%E4%B9%88%E9%80%89%E5%85%B6%E5%AE%83)
[2][超级强大的vim配置-vimplus](http://www.cnblogs.com/highway-9/p/5414465.html)
[3][超级强大的vim配置-vimplus(续集)](http://www.cnblogs.com/highway-9/p/5984285.html)
[4][打造一流编辑器vimplus](http://www.cnblogs.com/burningTheStar/p/6961220.html)

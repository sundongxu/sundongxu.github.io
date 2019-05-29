---
title: 终端利器 Zsh 配置全方略 (Linux与MacOS)
date: 2018-01-17 11:11:11
categories:
- Operating System
tags:
- Linux
- MacOS
- Shell
- Zsh
---

## 全文概要
---
**Zsh** 作为一款 **Terminal** 利器，使用方便，功能强大，社区活跃，并且同时适用于 **Linux** 与 **MacOS** 系统，本文详细介绍了在两种操作系统下 **Zsh** 的安装过程、文件配置及插件功能。
<!--more-->

## Shell
---
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

目前常用的 Linux 系统和 MacOS 系统的默认 Shell 都是 bash，但是真正强大的 Shell 是深藏不露的 **Zsh**， 这货绝对是飞机中的战斗机，但是之前由于配置过于复杂，所以初期无人问津，很多人跑过来看看 zsh 的配置指南，什么都不说转身就走了。直到有一天，国外有个穷极无聊的程序员开发出了一个能够让人快速上手的zsh项目，叫做[**oh-my-zsh**](http://ohmyz.sh/)，GitHub地址在[**这里**](https://github.com/robbyrussell/oh-my-zsh)，这玩意差不多就像是《XX天从入门到精通》系列，号称让人神功速成，而且居然是真的...

## 安装
---
在Linux系统下，这里以 **Ubuntu** 为例，**Zsh** 安装很方便：
{% codeblock %}
$ sudo apt-get install zsh git wget
$ wget --no-check-certificate https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh # 下载并安装Zsh
{% endcodeblock %}

安装 **oh-my-zsh** 可以自动安装也可以手动安装
{% codeblock %}
# 自动安装：
$ wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh
# 手动安装
$ git clone git://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh
$ cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc
{% endcodeblock %}

而在 **MacOS** 系统中，系统预装了 **Zsh**，那么只需要安装 **oh-my-zsh** 即可，下面的配置及插件使用过程两个系统都相同。

## 配置
---
安装完成，执行以下命令将 **Zsh** 设置为默认 **Shell**：
{% codeblock %}
# Ubuntu
$ chsh -s /usr/bin/zsh
# MacOS
$ chsh -s /bin/zsh
{% endcodeblock %}

注销一次，重新打开终端生效，查看此时使用的 **Shell**：
{% codeblock %}
$ echo $SHELL
{% endcodeblock %}
显示为 **Zsh** 即可。

**Zsh** 的配置主要集中在用户home目录的 **.zshrc** 里，可以在此处定义自己的环境变量和别名，当然，**oh my zsh** 在安装时已经自动读取当前的环境变量并进行了设置，你可以继续追加其他环境变量：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/zsh/environmental-variables.png)

接下来进行别名的设置，部分常用别名设置如下：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/zsh/alias.png)

设置完环境变量和别名之后，**Zsh** 基本上就很好用了。如果你是个主题控，还可以玩玩 **Zsh** 的主题。在 .zshrc 里找到ZSH_THEME，就可以设置主题了，默认主题是：
> ZSH_THEME="robbyrussell"

**oh my zsh** 提供了数十种主题，相关文件在 **~/.oh-my-zsh/themes** 目录下，默认提供了100多种，你可以随意选择，主题效果可在[**这里**]预览，也可以编辑主题满足自己的变态需求，我就用的默认主题 **robbyrussell**，大致效果如下：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/OS/Installation/Ubuntu/zsh/theme-robbyrussell.png)

## 插件
---
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
|   g       | git               |  命令原型                           |
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

这类工具中最早的应该是大名鼎鼎的 [**autojump**](https://linux.cn/article-3401-1.html)，GitHub项目地址在[**这里**](https://github.com/wting/autojump)，快捷命令是 **j**。**autojump** 使用Python编写，对于Bash和Shell的支持都比较好。但是可能是因为是Python写的吧，有的时候会感觉反应有些慢。详细配置参见这篇[**MacTalk**](http://macshuo.com/?p=676)。

有了 **j** 之后，又有了 **z**。**z** 的介绍就是"更好的 **j**"。它的功能和 **j** 基本是相同的，不过它使用Shell脚本编写，更加简洁，基本不会拖慢终端的响应速度。我比较喜欢简洁的，现在看来Github上大部分人也是, **j** 得到了3000+的star，超越了它的前辈 **autojump**。进一步了解请参见[**这篇**](http://www.zcfy.cc/article/become-a-command-line-power-user-with-oh-my-zsh-and-z)，详细用法参见 **z** 的[**Github项目地址**](https://github.com/rupa/z)。

然而人们还不满足，于是又有了大杀器 [**Fasd**](https://github.com/clvv/fasd)。**Fasd** 不光会记录目录，还会记录文件，也就是说它可以做到快捷打开某个深层目录的文件。**Fasd** 还可以通过配置，实现更加高级的功能。**Fasd** 与 **Zsh** 的结合也非常好，可以使用Tab键灵活的在几个目录中选择。可能是由于 **Fasd** 太强大了，虽然它使用Shell脚本写的，但是在使用的时候还是会感觉拖慢了终端的速度，特别是在执行 **ls -l** 的时候，会感觉输出明显慢了一拍。

三个工具各有各的特点，人们在追求命令行工作的效率上真的是永无止境的。如果有新的、更好的工具出现，欢迎留言告诉我，感谢赐教。

最后说一句，虽然 **oh-my-zsh** 提供了很多插件，不过也不要贪多哦，大量的插件会拖慢打开的速度，只安装常用的就好了。

## 上手
---
**Zsh** 兼容 **Bash**，切换过来毫无压力，该咋用咋用。但相比后者，**Zsh** 还具有以下特色：
1. **强大的历史纪录功能**：输入 grep 然后用上下箭头可以翻阅你执行的所有 grep 命令；
2. **智能拼写纠正**：输入gtep mactalk * -R，系统会提示：zsh: correct ‘gtep’ to ‘grep’ [nyae]? 比妹纸贴心吧，她们向来都是让你猜的……
3. **各种补全**：路径补全、命令补全，命令参数补全，插件内容补全等等。触发补全只需要按一下或两下 tab 键，补全项可以使用 ctrl+n/p/f/b上下左右切换。比如你想杀掉 java 的进程，只需要输入 kill java + tab键，如果只有一个 java 进程，zsh 会自动替换为进程的 pid，如果有多个则会出现选择项供你选择。ssh + 空格 + 两个tab键，zsh会列出所有访问过的主机和用户名进行补全；
4. **智能跳转**：安装了autojump之后，zsh 会自动记录你访问过的目录，通过 j + 目录名 可以直接进行目录跳转，而且目录名支持模糊匹配和自动补全，例如你访问过hadoop-1.0.0目录，输入j hado 即可正确跳转。j –stat 可以看你的历史路径库；5. **目录浏览和跳转**：输入 d，即可列出你在这个会话里访问的目录列表，输入列表前的序号，即可直接跳转；
6. **目录名直接跳转**：在当前目录下输入 .. 或 … ，或直接输入当前目录名都可以跳转，你甚至不再需要输入 cd 命令了；
7. **通配符搜索**：ls -l **/*.sh，可以递归显示当前目录下的 shell 文件，文件少时可以代替 find，文件太多就了；
8. **更强的别名**：请参考 **配置** 一节；
9. **插件支持**：请参考 **插件** 一节。

本小节内容写作时参考了多方资料，尤其是[[1]](http://macshuo.com/?p=676)，特此感谢！

## 参考资料
---
[1][终极Shell-MacTalk](http://macshuo.com/?p=676)
[2][j,z和fasd——Shell目录跳转工具](https://skyline75489.github.io/post/2014-12-13_j-z-and-fasd.html)
[3][Ubuntu下安装 Oh-My-Zsh 和 autojump](http://notes.11ten.net/zsh.html)
[4][通过 Oh-My-Zsh 和 Z 成为一个命令行高手](http://www.zcfy.cc/article/become-a-command-line-power-user-with-oh-my-zsh-and-z)
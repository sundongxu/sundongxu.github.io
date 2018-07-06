---
title: 库/头文件搜索路径相关GCC参数及环境变量
date: 2018-07-01 12:12:12
categories:
- Development
tags:
- C/C++
- GCC
- Environment Variable
- Linux
---

## 全文概要
---
在Linux系统中，命令行下编译C/C++代码是不二选择，**GCC**则是编译工具首选，提供了丰富的编译、调试特性，功能十分强大。本文介绍了使用**GCC**编译时，与头文件及库文件搜索路径相关的命令行参数及环境变量。

<!--more-->

高级语言，因为其抽象性所以用户友好，但也正因于此，需要翻译成机器指令才能让CPU理解并最终执行。
对于C/C++程序而言，翻译是一个过程，包含如下步骤：
1. 预处理(*.c -> *.i)
2. 编译(*.i -> *.s)
3. 汇编(*.s -> *.o)
4. 链接(*.o -> ELF)

链接最后生成一个二进制的可执行文件，装载入内存方可执行。各个步骤的具体细节正在学习之中，将在之后的博文中介绍。

本文重点关注于介绍在使用**GCC**编译时常用的一些命令行参数及所需配置的环境变量，如指定头文件搜索路径、静态库搜索路径、动态库搜索路径等。

## 实验平台
---
操作系统：Ubuntu 16.04.4
GCC版本：5.4.0

## 静态库
---
静态库文件命名以**"lib"**开头，后缀为**".a"**。
静态库代码在编译时期即载入可执行程序，但可能导致可执行文件的体积过大。

由**.o**文件创建静态库：
```
$ gcc -c hello.c
$ ac cr libhello.a hello.o # 静态库名为libhello.a
```

编译时链接静态库，假设静态库放在当前目录(.)下：
```
$ gcc -o hello main.c -L. -lhello
```

注意：**"-lhello"** 放置的位置很重要，因为``main.c``中用到了``libhello.a``中定义的方法``hello()``，则必须将**"-lhello"**放在``main.c``之后，否则**ld**报错：
```
$ gcc -o hello -L. -lhello main.c 
/tmp/xxxxxxxx.o: In function 'main':
main.c:(.text+0xa): undefined reference to 'hello'
collect2: error: ld returned 1 exit status
```

链接时，链接器**ld**会将静态库中的相关代码全部放入最终形成的可执行文件中，因此即使在得到可执行文件后立即删除静态库文件，执行可执行文件时也不会有任何问题，因为可执行文件所依赖的静态库相关代码全部以副本形式保存在了可执行文件中。

## 动态库
---
动态库文件命名以lib开头，后缀为.so.x.y，其中x为主版本号，y为副版本号。
共享库的代码是在可执行文件运行时才载入内存的，在编译过程中仅是简单地引用，因此可执行文件的体积较小。

C与C++的标准库均为动态库，在Ubuntu 16.04.4下，分别位于：
```
/lib/x86_64-linux-gnu/libc.so.6
/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.21
```

由**.o**文件创建动态库：
```
$ gcc -fPIC -c hello.c 
$ gcc -fPIC -shared libhello.so.1.2 hello.o # 要用"-fPIC"内容无关代码这一选项，必须在生成hello.o文件时也要指定该选项
```

编译时链接动态库，假设动态库放在当前目录(.)下：
```
$ gcc -o hello main.c -L. -lhello
```

注意：**"-lhello"**放置的位置很重要，理由和静态库部分一样。

执行：
```
$ ./hello
```

发现再次报错：
```
$ ./hello: error while loading shared libraries: libhello.so.1.2: cannot open shared object file: No such file or directory
```

根据错误提示可知：在运行时未能找到动态库``libhello.so.1.2``。你或许会问，编译都成功了，说明链接器确实找到了所依赖的动态库的位置，为什么在运行时却又找不到了呢？

这是因为：可执行文件在被装载运行时，会在系统默认的相关路径下去查找所依赖的动态库文件，如**/usr/lib**和**/lib**等目录。若能在这些路径下找到库，则将其装载，程序成功运行。否则就将出现上述错误导致程序终止。

解决方案：配置环境变量``LD_LIBRARY_PATH``，用于指定运行时的动态库的实际路径，使得程序在运行时能够找到并装载动态库。
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. # 在当前终端中临时配置环境变量，新增当前目录(即".")为动态库运行时搜索位置
```

这里其实还有另外一个环境变量``LIBRARY_PATH``，用于指定编译链接时的动态库的实际路径，效果类似于后文马上要介绍的**GCC**参数**"-Ldir"**，但两者同时存在时，会优先搜索**"-Ldir"**指定的路径。

此外，跟动态库搜路路径相关的还有一个配置文件``/etc/ld.conf.d``，也会指定一些库搜索路径，在Ubuntu 16.04.3系统下，该文件初始内容为：
```
include /etc/ld.so.conf.d/*.conf
```

进入到目录/etc/ld.so.conf.d下，发现有如下5个.conf文件：
```
- fakeroot-x86_64-linux-gnu.conf # /usr/lib/x86_64-linux-gnu/libfakeroot
- libc.conf # /usr/local/lib
- x86_64-linux-gnu.conf # /lib/x86_64-linux-gnu 和 /usr/lib/x86_64-linux-gnu
- x86_64-linux-gnu_EGL.conf -> /etc/alternatives/x86_64-linux-gnu_egl_conf
- x86_64-linux-gnu_GL.conf -> /etc/alternatives/x86_64-linux-gnu_gl_conf
```

注：在同名静态库和动态库同时存在时，**GCC**编译时默认优先链接动态库，但可通过**"-static"**选项强制其链接静态库。

**GCC**中与链接器**ld**相关的参数，还有一些参数也非常重要：
```
-rpath=dir，增加运行时库的搜索路径，即动态库的搜索路径，在编译链接和运行时均有效。
-rpath-link=dir，增加运行时库的搜索路径，即动态库的搜索路径，仅在编译链接时有效。
```

链接器**ld**会按照下列顺序搜索所需动态库：
1. 由"-rpath-link=dir"指定的目录dir
2. 由"-rpath=dir"指定的目录dir
3. 由环境变量LD_RUN_PATH指定的目录
4. 由环境变量LD_LIBRARY_PATH指定的目录
5. 由环境变量DT_RUNPATH或DT_RPATH指定的目录
6. 默认搜索路径，通常为/lib和/usr/lib
7. /etc/ld.so.conf中指定的目录

## 库文件路径
---
**GCC**默认的动态库搜索路径``SEARCH_DIR``可以通过**ld --verbose**命令查看。

**GCC**在编译文件时通过如下参数可新增库的搜索路径：
```
-Ldir：链接时在目录dir中查找由**"-l"**参数指定的库
-llibray：链接时搜索名称为library的库，该选项在编译整个命令中的位置会表现出不同(make difference)，链接器ld会按照其书写的顺序搜索和处理库和目标文件(*.o)
```

使用实例参考上一节中在编译``main.c``新增``libhello.a``或``libhello.so.1.2``库的路径：
```
$ gcc -o hello main.c -L. -lhello
```

那么我们如何知道一个可执行文件运行时依赖哪些库呢？使用**ldd**命令：
**ldd**命令可以查看一个可执行程序依赖的共享库，该命令不仅会输出共享库的名称，还有共享库在系统中的所在路径和在可执行文件(ELF)中的地址(0x0000xxxxxxxxxxxx，16*4=64 bit 地址)，对C++的标准库执行该命令，会发现其依赖于C标准库``libc.so``(即glibc)。

## 头文件
---
在C/C++程序中，头文件(*.h)即对外暴露的API接口，使用者通过在程序开始处**#include**头文件，即可使用头文件中声明的类与方法，而头文件中仅有类和方法的声明，定义则通常在打包在库中的源文件(*.c/*.cpp/*.cc)中。

**GCC**头文件搜索路径相关参数：
```
-I dir，增加头文件搜索路径dir，-I指定的目录会在系统默认搜索路径之前被搜索
-Idir，增加头文件搜索路径dir，是头文件搜索的第一顺位选择，多个-I参数则按照从左到右的顺序依次进行搜索
-isystem dir，增加头文件搜索顺序dir，顺序排在在"-I"之后，但是在系统默认搜索路径之前
```

"-I dir"是"Options Controlling the Preprocessor"，而"-Idir"是"Options for Directory Search"，效果等同。

相关环境变量则有``C_INCLUDE_PATH``和``CPLUS_INCLUDE_PATH``，也可以通过配置这两个变量新增头文件搜索路径，使得C和C++源程序中能够找到其**include**的头文件。

注：用**GCC**参数选项指定的目录会在由环境变量指定的目录之前被优先搜索。什么参数都不指定的时候，**GCC**会优先搜索当前工作目录，以查找头文件和库。

## 参考资料
---
[1][Linux 动态库与静态库](https://blog.csdn.net/yuleslie/article/details/7695102)
[2][LIBRARY_PATH与LD_LIBRARY_PATH的区别](https://typecodes.com/cseries/gcclderrlibrarypath.html)
[3][ld Man Page](http://man7.org/linux/man-pages/man8/ld.so.8.html)
[4][GCC Environment Variables](https://gcc.gnu.org/onlinedocs/gcc/Environment-Variables.html)
[5][LD_LIBRARY_PATH VS LIBRARY_PATH - StackOverFlow](https://stackoverflow.com/questions/4250624/ld-library-path-vs-library-path)
[6][GCC指定头文件和库文件搜索路径](https://blog.csdn.net/mybelief321/article/details/9099659)
[7][C++动态库与静态库](https://blog.csdn.net/blognkliming/article/details/45980391)
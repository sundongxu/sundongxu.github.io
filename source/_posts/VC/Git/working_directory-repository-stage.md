---
title: Git工作区、版本库与暂存区
date: 2017-10-31 21:40:12
categories:
- Version Control
tags:
- Git
- Working Directory
- Repository
- Stage
---

## 全文概要
Git与其它版本控制系统如SVN的一个最大的不同之处就是发明了暂存区的概念，本文从创建Git版本库开始，依次描述了追踪文件、修改文件、丢弃修改、提交修改等基本Git操作，在此过程中介绍了Git版本控制系统中三个重要概念：工作区、版本库与暂存区。

<!--more-->

## 工作区(Working Directory)
---
就是你能在电脑中看到的目录，即本地目录，比如我的**GitTest**就是一个工作区，准确的来说，Git工作区应该指的是执行过git init初始化后的本地目录，下图可以看到现在工作区内只有一个文件:a.txt：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/0-git-init.png)

## 版本库(Repository)
---
工作区有一个隐藏目录**.git**，这个不是工作区，而是Git的版本库。
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/repository.png)

Git版本库里存了很多东西，其中最重要的就是称为"Stage"的暂存区，还有Git为我们自动创建的第一个本地分支Master。

分支与HEAD的概念之后的博文会详细讲解。

## 暂存区(Stage)
---
前面说到暂存区实际是Git版本库里面的一个区域，具体的结构参见下图：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/working_directory-repository-stage.png)

下文中会结合一次完整的版本控制操作过程来具体讲解各个区域的作用。

## 本地版本控制流程实例
---
实例开始之前在强调一下，使用git命令时，status绝对是一个功能超乎想象重要的参数，让使用者可以获得整个版本文件的实时视图。

流程按照上图中显示的git版本库继续操作，所有操作均在本地分支Master上完成：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/1-untracked-file.png)

工作区中包含未被追踪的文件(Untracked files)：**a.txt**，按提示使用命令：**git add <文件名>**，就可以将该文件添加至**暂存区**，并使用命令**git status**，查看当前版本完整视图：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/2-git-add.png)

根据提示使用命令：**git rm - -cached <文件名>**，可取消缓存在暂存区的文件修改：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/3-git-rm-cached.png)

发现暂存区的文件被弹出，**a.txt**重新回到了**Untracked files**下，再次执行**git add**将其加入暂存区：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/4-git-add-again.png)

接下来使用命令：**git commit -m "message"**，将暂存区文件提交，此时终端显示“nothing to commit, working directory clean”(没有可提交的内容，工作区是干净的)，这是因为工作区中的文件修改已全部提交至版本库中的本地分支Master上，工作区与Master分支内容完全一致，即没有“脏”内容(类比数据库的“脏读”(Dirty Read)，脏读是指一个事务读到了另一个事务还未提交的数据，实际就是存在有未提交的数据)的情况：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/5-git-commit.png)

下面我们试着在工作区创建一个新文件**b.txt**，此时显然该文件属于**Untracked files**：

    b.txt
    这是第一次修改.

![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/6-git-touch-new-file.png)
同样将其添加到暂存区中：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/7-git-add.png)

如果你足够细心，不难发现一些**有意思的事情**：同样是追踪文件即将文件修改加入暂存区，两次执行**git add**命令的效果一样，但是Git给出的取消文件暂存命令提示不太一样：

- 将**a.txt**加入暂存区时，Git提示取消暂存的命令为：**git rm - -cached <文件名>**。

- 将**b.txt**加入暂存区时，Git提示取消暂存的命令为：**git reset HEAD <文件名>**。

出现这种差异是因为**a.txt**属于第一次提交(Initial Commit)的文件内容，对应取消暂存的命令就是：**git rm - -cached <文件名>**。而如果在初始化版本库(即**git init**)之后新增的文件，取消暂存的命令则是：**git reset HEAD <文件名>**。

那我们执行取消暂存**b.txt**的命令：**git reset HEAD b.txt**，发现**b.txt**重新回到**Untracked files**类别下：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/8-git-reset-HEAD-unstage.png)

再次将**b.txt**添加到暂存区：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/9-git-add.png)

然后我们在最后提交之前再对**b.txt**进行修改，增加一行内容：

    b.txt
    这是第一次修改.
    这是第二次修改.

![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/10-git-modify.png)

发现**b.txt**同时在暂存区(Changes to be committed)和未暂存区(Changes not staged for commit，为了与暂存区对比，姑且这么称呼，实际上就是工作区，只不过对应文件在上一次被暂存后又再次被修改过，而修改过的文件内容还未被暂存)，但是描述有些许不同：

- 在暂存区中，**b.txt**前面修饰词为**new**，表示对于暂存区而言，这是**b.txt**第一次被加入到暂存区中。

- 在未暂存区中，**b.txt**前面修饰词为**modified**，表示对于为暂存区而言，它已经知道**b.txt**之前已经被暂存过一次，而在那之后又被修改过。

对于状态下的**b.txt**文件，Git都给出了明确的提示，我们挨个尝试一遍。

直接取消上一次暂存：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/11-git-reset-HEAD-unstage.png)

再次暂存**b.txt**：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/12-git-add.png)

再次修改**b.txt**，增加一个文本行：

    b.txt
    这是第一次修改.
    这是第二次修改.
    这是第三次修改.

![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/13-git-modify.png)

执行命令：**git checkout - - <文件名>**，该命令效果应该是丢弃本地修改：

![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/14-git-checkout-discard-changes.png)

**cat**命令查看**b.txt**内容发现上次对**b.txt**的修改(新增一个文本行)果然被丢弃。

再次修改**b.txt**内容，增加一个空行和一个文本行：

    b.txt
    这是第一次修改.
    这是第二次修改.
    
    这是第四次修改.

![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/15-git-modify.png)

这一次我们直接将修改后的**b.txt**添加至暂存区：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/16-git-add.png)

提交暂存区到本地分支Master：
![image](https://raw.githubusercontent.com/sundongxu/blog-img-hosting/master/images/VC/Git/working_directory-repository-stage/17-git-commit.png)

以上，我们就完成了一次本地分支的版本控制。

## 小结
---
1. **工作区**：本地目录，包含所有被Git追踪的文件的实时内容；
2. **版本库**：工作区中的.git文件夹，实际包含暂存区和当前分支指针HEAD，通过命令：**git init**可以初始化创建版本库；
3. **暂存区**：暂存被追踪的文件修改，为正式提交到本地分支提供内容。

- **工作区**文件修改后通过命令：**git add <文件名>**，将文件添加至**暂存区**。
- **暂存区**通过命令：**git rm --cached <文件名>**(针对在初始化版本库之前就已经在**工作区**中的文件)，或**git reset HEAD <文件名>**(针对在初始化版本库之后才新增的文件)，将暂存文件弹回到**工作区**。
- 若要在**工作区**丢弃文件自上一次暂存以来的全部修改，可执行命令：**git checkout -- <文件名>**。

## 参考资料
---
[1] [廖雪峰Git教程](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013745374151782eb658c5a5ca454eaa451661275886c6000)

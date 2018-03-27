---
title: Ubuntu + GitHub Pages + Hexo 搭建个人博客
date: 2018-01-25 13:13:13
categories:
- Operating System
tags:
- Ubuntu
- GitHub
- Hexo
- Blog
---

## 全文概要
---
本博客站点采用 **GitHub Pages** + **Hexo** 框架搭建，其中 [**GitHub Pages**](https://pages.github.com/) 作为内容托管平台，[**Hexo**](https://hexo.io/zh-cn/index.html) 是一款静态博客框架，提供了多种多样的主题与插件，使得用户可以对个人站点进行个性化定制及管理，支持归档、分类、标签、搜索、资源管理等基本功能。用户可以在本地的博客项目中新建 [**Markdown**](http://wowubuntu.com/markdown/) 文档编辑博文内容，并利用 **Hexo** 提供的命令行工具将 **Markdown** 文档转换为个人网络站点所展示的 **HTML** 页面。本文详细介绍了博客框架的配置全记录。
<!--more-->

## GitHub
---
**[Github Pages](https://pages.github.com/)** 是 **GitHub** 公司提供的免费的静态网站托管服务，用起来方便而且功能强大，不仅没有空间限制，还可以绑定自己的域名。**Git** 及 **GitHub** 的配置与使用就不多说了，将博客托管到 **GitHub** 上，首先需要新建仓库，名称必须为：
> **username.github.io**

{% qnimg OS/Installation/Ubuntu/blog-github-hexo/sundongxu.github.io.png %}

这是特殊的命名约定，创建完毕后可以通过 **[http://username.github.io](dongdongdong.me)** 来访问你的个人主页。

特别提醒：个人主页的网站内容是发布在master分支下的。

## Hexo
---
**Hexo** 出自台湾大学生 **tommy351** 之手，基于 **Node.js**，其编译上百篇文字只需要几秒，并且简单易于配置，很适合搭建个人站点。**Hexo** 生成的静态网页可以直接部署到 **GitHub Pages** 等平台上。

### Nodejs
编译最新版本 **Nodejs (9.5.0)**：
{% codeblock %}
$ wget https://nodejs.org/dist/v9.5.0/node-v9.5.0.tar.gz
$ tar zxvf node-v9.5.0.tar.gz
$ cd node-v9.5.0
$ ./configure --prefix=/usr/local/node
$ make -j
$ sudo make install
{% endcodeblock %}

配置环境变量：
{% codeblock %}
$ vim .zshrc
# 将以下内容添加到末尾
export NODE_HOME=/usr/local/node
export PATH=$PATH:$NODE_HOME/bin
$ source .zshrc
$ node -v # v9.5.0
$ npm -v  # 5.6.0
{% endcodeblock %}

### NPM
**NPM (Node Package Manager)** 是随同 **Nodejs** 一起安装的包管理工具，能解决 **Nodejs** 代码部署上的很多问题，常见的使用场景有以下几种：
- 允许用户从NPM服务器下载别人编写的第三方包到本地使用
- 允许用户从NPM服务器下载并安装别人编写的命令行程序到本地使用
- 允许用户将自己编写的包或命令行程序上传到NPM服务器供别人使用

**NPM** 安装 **Nodejs** 模块语法如下：
{% codeblock %}
$ npm install <module_name>
{% endcodeblock %}

安装好之后，模块包就被放在了工程目录下的 **node_modules** 目录中。

** NPM ** 的包安装分为 **本地安装 (local)** 和 **全局安装 (global)** ，命令分别如下：
{% codeblock %}
$ npm install <module_name>    # 本地
$ npm install <module_name> -g # 全局
{% endcodeblock %}

两种安装方式的对比如下：
> **本地安装**
（1）将安装包放在 ./node_modules 下 (运行 npm 命令时所在的目录)；
（2）可以通过 require() 来引入本地安装的包。

> **全局安装**
（1）将安装包放在 /usr/local/lib 下或者你 node 的安装目录；
（2）可以直接在命令行里使用。

其它常用 **NPM** 命令如下：
{% codeblock %}
$ npm uninstall <module_name> # 卸载
$ npm update <module_name>    # 更新
$ npm search <module_name>    # 搜索
$ npm list <module_name>      # 查看版本号
$ npm list -g                 # 查看所有全局安装的模块
{% endcodeblock %}

国内直接使用 **NPM** 的官方镜像是非常慢的，推荐使用淘宝 **NPM** 镜像，同步频率为10分钟一次，更换镜像源命令如下：
{% codeblock %}
$ npm install -g cnpm --registry=https://registry.npm.taobao.org
{% endcodeblock %}

模块的描述文件：**package.json**，它位于各模块的目录下，用于定义包的相关属性，部分字段说明如下：
- name - 包名
- version - 包版本号
- description - 包的描述
- homepage - 包的官网 url
- author - 包的作者姓名
- contributors - 包的其他贡献者姓名
- dependencies - 依赖包列表。如果依赖包没有安装，npm 会自动将依赖包安装在 node_module 目录下
- repository - 包代码存放的地方的类型，可以是 git 或 svn，git 可在 Github 上
- main - main 字段指定了程序的主入口文件，require('moduleName') 就会加载这个文件。这个字段的默认值是模块根目录下面的 index.js
- keywords - 关键字

### Hexo
使用 **NPM** 安装 **Hexo**：
{% codeblock %}
$ npm install -g hexo-cli # 可能需要 sudo 权限
{% endcodeblock%}

#### 建站
**Hexo** 安装完成后，初始化博客目录，输入：
{% codeblock %}
$ hexo init blog
$ cd blog
$ npm install
{% endcodeblock %}

**Hexo** 会在指定文件夹中新建需要的文件，完成后，该文件夹下的目录文件说明如下：
> **_config.yml**：站点配置文件
**package.json**：Hexo应用程序信息
**scaffolds**：模板文件夹，根据此文件夹下的.md文档新建页面、文章、草稿
**source**：用户资源文件夹，_post保存博文文档，Markdown 和 HTML 文件会被解析并放到 public 文件夹，而其他文件会被拷贝过去。分类、标签、关于页面模板分别放在 categories、tags 和 about 下
**themes**：主题文件夹，Hexo会根据主题来生成静态页面

#### 配置
你可以在 **_config.yml** 中修改大部分站点配置，详见[**官方文档-Hexo配置**](https://hexo.io/zh-cn/docs/configuration.html)

#### 命令
基本 **Hexo** 命令如下：
{% codeblock %}
$ hexo new "post_name" # 新建博文，.md文档生成保存在source/_post目录下
$ hexo generate/g      # 载入全部.md文件，生成可供浏览器展示的HTML静态页面
$ hexo server/s        # 将本机作为托管服务器，在本地启动站点，可通过 localhost:4000 访问博客
{% endcodeblock %}

在某些情况(尤其是更换主题后)，如果发现对站点的更改无论如何也不生效，可能需要运行 **clean** 命令：
{% codeblock %}
$ hexo clean # 清除缓存文件 (db.json) 和已生成的静态文件 (public)
{% endcodeblock %}

最终博客文件是要被托管到 **GitHub** 上的，确认已经创建名为 **username.github.io** 的仓库后，执行：
{% codeblock %}
$ npm install hexo-deployer-git --save 
{% endcodeblock %}  

安装好插件后编辑 **_config.yml** 文件，翻到最后，找到 **deploy** 字段，改成如下格式：
{% codeblock %}
deploy:
    type: git
    repo: https://github.com/username/username.github.io.git
    branch: master
{% endcodeblock %}

配置完成后，**Hexo** 框架下的博客文件就可以通过以下命令轻松部署并发布到 **GitHub** 对应仓库中的 **master** 分支上：
{% codeblock %}
$ hexo deploy/d
{% endcodeblock %}

到这里，你的博客就已经能够被搜索引擎索引到，或直接访问 **http://username.github.io**，供人查阅：

除开官网上的基本配置，**Hexo** 更是提供了丰富的个性化选项，例如 更换主题、流量统计、评论分享、站内搜索 ... 太适合折腾了有木有！

不过，“弱水三千，只取一瓢饮”，点到为止即可，毕竟写博客的初衷一直都是记录、积累和总结知识呀。

当然啦，伸手党也是要满足的，[**这篇**](https://segmentfault.com/a/1190000009544924)拿去不谢～

## 图片托管
---
Hexo文章中的图片，当然可以放到本地，在博文中通过相对路径引用，然后一起部署到github中，这样完全没有问题。然而 **GitHub Pages** 空间毕竟有限（貌似只有300M）,另外图片的管理太混乱了，一些原创的图片可能被盗链。七牛作为国内顶尖的 **CDN** 云存储商，有以下几个重要优势：
- 在国内很稳定，我们公司也是选择七牛来提供云存储的
- 免费提供10G存储空间，和每月10G下载流量，完全够用
- Hexo有七牛的插件，使用起来也是相当的方便

### 申请空间
点击[**此处**](https://portal.qiniu.com/signup?code=3lbz03l9r6c7m)申请七牛账号，随后点击 对象存储(立即添加) —> 创建公开空间，记录七牛为该存储空间分配的域名，比如我的空间名为 **dongdongdong-blog**，域名为 **oyj8xdeki.bkt.clouddn.com**。

点击右上角 **个人面板** —> 密钥管理，复制当前使用的 **AK (AccessKey)** 和 **SK (SecretKey)**，待会儿设置插件时会用到。

### 安装插件
**Hexo** 安装七牛插件：
{% codeblock %}
$ npm install hexo-qiniu-sync --save
{% endcodeblock %}

将插件配置信息写入站点配置文件 **_config.yml**：
{% codeblock %}
plugins: 
    -hexo-qiniu-sync
qiniu:
    offline: false
    sync: true
    bucket: dongdongdong-blog(存储空间名称)
    access_key: 上一步记录的 AK 值
    secret_key: 上一步记录的 SK 值
    dirPrefix: 
    urlPrefix: oyj8xdeki.bkt.clouddn.com(空间域名)
    local_dir: cdn(本地目录名称)
    update_exist: true
    images:
        folder: images
        extend:
    js:
        folder: js
    css: 
        folder: css
{% endcodeblock %}

### 配置目录
图片一般都先保存在本地，然后再通过上述插件同步到七牛云。

本地目录名称需要与配置项 **local_dir** 一致，我将其命名为 **cdn**，并与博客主目录下的 source 文件夹平级，用于存放需要上传到七牛的资源。

在 **cdn** 目录下创建子目录：**css**、**images**、**js**，与 **image/js/css** 字段的子参数 **folder** 保持一致，待上传的 **css**、**图片**、**js** 文件应该存储到相应子目录。

### 引用资源
以图片引用为例，这也是最常见的情形。如果你想引用存储在 **cdn/images** 下的图片 **demo.jpg**，只需在文章中插入：
```
{% qnimg demo.jpg %}  # 如果 cdn/image 目录下有更深层子目录，则加上后面的路径
```

生成网页时将被解析为：
{% codeblock %}
<img src="http://oyj8xdeki.bkt.clouddn.com/images/demo.jpg">·
{% endcodeblock %}

将引用实例概括为：
```
{% qnimg ImgFile %}
```

注意到上面七牛插件配置文件的 **urlPrefix** 和 **images** 的 **folder** 子参数，不难发现 **图片外链地址** 的生成规则为：
> urlPrefix + / + images.folder + / + ImgFile

定义文件在 **cdn** 目录下的 **相对路径** 为 **FilePath** ，即文件 **cdn/demo.jpg** 的  **FilePath** 为 **demo.jpg**，**cdn/images/test/01.png** 的 **FilePath** 为 **images/test/01.png**。则实际 **上传到七牛的文件外链地址** 为：
> AK/SK所指向用户的绑定域名 + / + dirPrefix + / + FilePath

只有当生成的图片外链地址与实际上传到七牛的文件外链地址一致时，引用的图片才能正确显示。

### 同步上传
每当将新资源文件放入需要同步的文件夹 **cdn** 中后，执行 **hexo s** 即启动站点服务器即可完成资源上传到七牛云，但有时候会莫名失效...

若上述命令没能成功上传，执行：
{% codeblock %}
$ hexo qiniu sync
{% endcodeblock %}

一定可以将资源同步到云端。

## 多机同步
---
执行：
{% codeblock %}
$ hexo deploy/d
{% endcodeblock %}

即可将本地博客内容发布到 **GitHub** 上，其中 **仓库** 与 **分支名** 由 **_config.yml** 中的 **deploy** 字段的子参数 **repo** 和 **branch** 显式指定。

此时在 **GitHub** 个人仓库 **username.github.io** 的 **master**分支的页面即可看到上传记录：
{% qnimg OS/Installation/Ubuntu/blog-github-hexo/sundongxu.github.io-publish.png %}

不难发现这个博客仓库是不包含原始文件的，比如博文对应的 **.md** 文件，而该文件都只保存在配置 **Hexo** 的机器本地，而上传到 **GitHub** 的只是转换渲染过后的 **.html** 网页文件。那么这就带来一个问题：如何能在原始机器以外的机器上继续编辑博文(.md文档)呢？

这就是一个多设备同步的问题，利用版本控制，思路如下：

1. 电脑A搭建好 **GitHub** 博客
2. 编译好的静态网页提交到主分支 **master**，提供访问
3. 创建一个分支 **hexo**，保存网站的源文件
4. 在电脑B上拉取 **hexo** 分支源文件，做更新博客操作
5. 编译博客，静态文件提交到主分支 **master**，源文件提交到 **hexo** 分支
6. 回到电脑A，同步 **hexo** 分支源文件到本地，开始更新博客

下面介绍每一步的具体操作：
第一步：在电脑A处，创建名为 **username.github.io** 的 **GitHub** 仓库；

第二步：编辑 **_config.yml** 设置 **deploy** 参数的子参数 **repo** 和 **branch** 分别为第一步中创建的仓库名和主分支 **master**；

第三步：在 **GitHub** 仓库中创建新分支 **hexo**，并将部分站点源文件 (如source文件夹，最好写个 **.gitignore** 文件) 提交到该分支，在博客根目录下执行命令：
{% codeblock %}
$ rm -rf themes/.git   # Git项目内不能再包含Git子项目
$ git init
$ git checkout -b hexo # 创建本地分支hexo 
$ git add .
$ git commit -m "create a new branch for coordination among multiple devices" 
$ git push origin hexo
{% endcodeblock %}

第四步：在电脑B处拉取分支hexo，做更新博客操作，先需搭建环境：
{% codeblock %}
$ git clone -b hexo https://github.com/username/username.github.io.git
$ cd username.github.io
$ npm install # 安装依赖
{% endcodeblock %}

此后就可以在电脑B上编辑更新博文了。

第五步：编译博客，将静态文件发布到主分支 **master** 上，源文件提交到分支 **hexo** 上：
{% codeblock %}
$ hexo clean && hexo g && hexo d
$ git add .
$ git commit -m "message"  # 源文件提交到hexo分支上面
$ git pull origin hexo     # 先拉取原来GitHub的hexo分支上的源文件到本地，进行合并
$ git push origin hexo     # 比较解决前后版本冲突后，push源文件到GitHub的hexo分支
{% endcodeblock %}

第六步：再次回到电脑A进行博文编辑工作，同步hexo分支源文件到本地，进行合并：
{% codeblock %}
$ git pull origin hexo
# 写好博文，重复操作第五步
{% endcodeblock %}

最后提醒一点，**Git** 带来的版本控制思想除了方便协同工作以外，还间接起到了文件备份的作用，所以无论在何处编辑好分支文件后，都请务必记得将本次修改内容提交到远端分支。对于博客工作而言，你是在正式发布博文 (hexo d) 之前还是之后，将源文件提交到 **hexo** 分支，这顺序不重要，不要忘记这一步就好。

## 域名管理
---
安装前文搭建好的博客站点，已经可以通过域名 **http://username.github.io** 被公网访问到，这个是由 **GitHub Pages** 免费提供的域名服务，当然你还可以通过购买配置后使用付费的专有域名，本博客域名  [**dongdongdong.me**](dongdongdong.me) 就是我在腾讯云上购买的，新用户或是学生还经常有优惠，第一年使用权都很便宜~

购买域名后，需进行实名认证，之后还要有一些配置工作，才能使该域名能够被正常解析并被定向至博客站点。

绑定域名需要在 域名解析服务商 和 **GitHub** 两边都进行操作。
1. 在域名解析服务商进行个人域名解析，将域名绑定到个人 GitHub Pages。
2. 同时在 GitHub Pages 需要配置 CNAME 文件重定向到你的域名。

下面以我的域名 [**dongdongdong.me**](dongdongdong.me) 为例，具体步骤如下：

**第一步**：在博客的 **source** 下新增一个 **CNAME (一定大写)** 文件，其中只能包含一个顶级域名：
```
dongdongdong.me
```

并发布到 **GitHub Pages** 上，完成 **GitHub** 一侧的配置操作：
``` 
$ hexo clean && hexo g && hexo s
```

本步骤**替代方案**：直接设置 **GitHub Pages** 的 **Custom Domain (自定义域)**：
进入名为 **username.github.io** 的仓库的设置(Setting)页面，在 **Custom Domain** 下面的方框里填入域名：
{% qnimg OS/Installation/Ubuntu/blog-github-hexo/setting-custom-domain.png %}

**第二步**：向你的 **DNS** 配置中添加三条记录：

|  主机记录  |  记录类型  |         记录值        |
| :------- |  :------- |  :------------------ |
| @        |  A        |  192.30.252.153      |
| @        |  A        |  192.30.252.154      |
| www      |  CNAME    |  sundongxu.github.io |

有两种配置 **DNS** 解析记录的方式：
（1）[**腾讯云**](https://cloud.tencent.com/)
> 腾讯云主页 -> 控制台 -> 域名管理 -> 选择指定域名 -> 域名解析 -> 添加记录

{% qnimg OS/Installation/Ubuntu/blog-github-hexo/dns-tencent-cloud.png %}

（2）[**DNSPOD**](https://www.dnspod.cn/)
> DNSPOD主页 -> 管理控制台 -> 已开通的服务 -> 域名解析 -> 选择指定域名 -> 添加记录

{% qnimg OS/Installation/Ubuntu/blog-github-hexo/dns-dnspod.png %}

配置完成后你会发现，无论按照哪一种方式添加 **DNS** 记录，都会在另一种方式的网页上被同步添加，其实腾讯云与**DNSPOD**早就打通数据通道啦。

第三步：等待 **DNS** 配置生效。
对 **DNS** 的配置不是立即生效的，过10分钟左右再去访问你的域名看看是否成功定向到你的 **GitHub Pages** 个人站点。

## 参考资源
---
[1][Hexo文档-配置](https://hexo.io/zh-cn/docs/index.html)
[2][Ubuntu + Github + Hexo搭建blog小记-magicse7en](http://magicse7en.github.io/2016/03/06/ubuntu-github-hexo-blog-setup/)
[3][通过Hexo在GitHub上搭建博客](https://www.jianshu.com/p/ecd51e8ef2fa)
[4][GitHub+Hexo搭建个人网站详细教程-知乎专栏](https://zhuanlan.zhihu.com/p/26625249)
[5][使用七牛为Hexo存储图片等资源](https://yuchen-lea.github.io/2016-01-21-use-qiniu-store-file-for-hexo/#fn.1)
[6][多设备更新hexo搭建的GitHub博客](https://zhangangs.github.io/2017/05/22/%E5%A4%9A%E8%AE%BE%E5%A4%87%E6%9B%B4%E6%96%B0hexo%E6%90%AD%E5%BB%BA%E7%9A%84github%E5%8D%9A%E5%AE%A2/)
[7][GitHub怎么绑定自己的域名？](https://www.zhihu.com/question/31377141)

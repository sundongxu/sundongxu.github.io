---
title: 数据结构之线性表
date: 2018-04-11 10:10:10
categories:
- Algorithm
tags:
- Data Structure
- Linear List
- Linked List
- C++
---

## 全文概要
---
线性表实现有两种方式，一种为**顺序表**，另一种为**链表**。本文分别介绍了**顺序线性表**、**单向链表**、**双向链表**和**循环链表**的基本结构，并给出了相应的C++类代码实现。
<!--more-->

## 线性表(Linear List)
---

### 顺序表(Sequential List)
---
在顺序实现中，数据存储在一个长度为`maxSize`，数据类型为`ElemType`的**数组**中，并用`count`记录在数组中的线性表的实际元素个数。由于顺序表本质是个数组，故其中逻辑位置连续的结点，其物理存储空间也连续。

顺序表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**SqList.h**](https://github.com/sundongxu/data-structure/blob/master/SqList.h)|[**SqList.cc**](https://github.com/sundongxu/data-structure/blob/master/SqList.cc)|

### 单链表(Linked List)
---
单链表是一种最简单的线性表的链式存储结构，单链表也称为线性链表，用它来存储线性表时，每个数据元素用一个结点(`node`)来存储，一个结点由两个域组成，一个是存放数据元素的`val`，称为**数据域**，一个是存储指向此链表下一个结点的指针`next`，称为**指针域**。

单链表结点的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**Node.h**](https://github.com/sundongxu/data-structure/blob/master/Node.h)|[**Node.cc**](https://github.com/sundongxu/data-structure/blob/master/Node.cc)|

单链表在表头通常会增加一个没有存储数据元素的结点，称之为"**头结点**"，在单链表中增加头结点虽然增加了存储空间，但算法实现更简单，效率更高。头结点的地址可从指针`head`找到，指针`head`也称为"**头指针**"，其它结点的地址则由其前驱结点的`next`域得到。

单链表用结点中的指针域来表示数据元素之间的逻辑关系，这样逻辑上相邻的两个元素并不要求物理存储位置也相邻。即在链表中，逻辑位置连续的结点，物理存储空间不必连续

#### 简单线性链表(Simple Linked List) 
线性链表简单实现为数据成员只有**头指针**。

简单线性链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**SimpleLinkList.h**](https://github.com/sundongxu/data-structure/blob/master/SimpleLinkList.h)|[**SimpleLinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/SimpleLinkList.cc)|

#### 线性链表(Link List)
单链表简单实现基础上增加了表示当前位置的序号`curPosition`，指向当前位置的指针`curPtr`，以及元素总个数`count`。

线性链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**LinkList.h**](https://github.com/sundongxu/data-structure/blob/master/LinkList.h)|[**LinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/LinkList.cc)|

### 双向链表(Double Linked List)
---
前面介绍的单链表的结点结构中只有一个指向后继的指针域，即`next`，这样便只能从左往右进行查找其它结点，如要查找前驱，则只有从表头出发进行查找，效率较低，双向链表通过在其结点结构中存储两个指针域`back`和`next`，分别指向该结点前驱和后继。

双向链表结点的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**DblNode.h**](https://github.com/sundongxu/data-structure/blob/master/DblNode.h)|[**DblNode.cc**](https://github.com/sundongxu/data-structure/blob/master/DblNode.cc)|

#### 简单双向链表(Simple Double Linked List)
简单双向链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**SimpleDbLinkList.h**](https://github.com/sundongxu/data-structure/blob/master/SimpleDbLinkList.h)|[**SimpleDbLinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/SimpleDbLinkList.cc)|

#### 双向链表(Double Linked List)
在双向链表简单实现基础上增加了表示当前位置的序号`curPosition`，指向当前位置的指针`curPtr`，以及元素总个数`count`。

双向链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**DbLinkList.h**](https://github.com/sundongxu/data-structure/blob/master/DbLinkList.h)|[**DbLinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/DbLinkList.cc)|

### 循环链表(Circular Linked List)
---
循环链表是另一种形式的线性表链式存储结构，它的结点结构与普通单链表相同，不同的是在循环链表中尾结点的`next`域不为空，而是指向起头结点，这样就将单链表首尾相接成为一个环。故而，循环链表判空的条件为：`head->next == head`。

#### 简单循环链表(Simple Circular Linked List)
简单循环链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**SimpleCircLinkList.h**](https://github.com/sundongxu/data-structure/blob/master/SimpleCircLinkList.h)|[**SimpleCircLinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/SimpleCircLinkList.cc)|

#### 循环链表(Circur Linked List)
在循环链表简单实现基础上增加了表示当前位置的序号`curPosition`，指向当前位置的指针`curPtr`，以及元素总个数`count`。

循环链表的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**CircLinkList.h**](https://github.com/sundongxu/data-structure/blob/master/CircLinkList.h)|[**CircLinkList.cc**](https://github.com/sundongxu/data-structure/blob/master/CircLinkList.cc)|

## 参考资料
---
[1][数据结构与算法(C++版) - 唐宁九主编](https://www.amazon.cn/dp/B001TREOXM)
[2][数据结构(用面向对象方法与C++语言描述) - 殷人昆主编](https://www.amazon.cn/dp/B0011F7UHO)
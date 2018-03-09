---
title: 数据结构之二叉搜索树
date: 2018-01-23 10:10:10
categories:
- Algorithm
tags:
- Data Structure
- Binary Search
- Java
---

## 全文概要
---
二叉搜索树(Binary Search Tree)，或称二叉查找树，是一种能够将链表插入的灵活性和有序数组查找的高效性结合起来的一种数据结构。本文详细介绍了二叉搜索树的基本概念、数据结构及相关算法实现，并在最后给出该结构的性能分析。
<!--more-->

## 术语
---
**二叉搜索树**，由 **结点** 构成：

{% codeblock %}
public class BST<Key extends Comparable<Key>, Value>
{
    private Node root;                  // 树根结点

    private class Node                  // 结点类
    {
        private Key key;                // 键
        private Value val;              // 值
        private Node left, right;       // 指向当前结点左右子树的链接
        private int N;                  // 以该结点为根的子树中的结点个数
    }

    public int size()
    {   return size(root);  }

    public int size(Node x)
    {
        if (x == null) return 0;
        else           return x.N;
    }

    // public Value get(Key key);
    // public 
}
{% endcodeblock %}

## 基本实现
---

## 
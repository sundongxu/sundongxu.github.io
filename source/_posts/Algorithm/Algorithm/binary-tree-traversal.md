---
title: 常见算法之二叉树遍历
date: 2018-04-23 12:12:12
categories:
- Algorithm
tags:
- Algorithm
- Binary Tree
- Traversal
- Pre-Order
- In-Order
- Post-Order
- Recursion
- NonRecursion
---

## 全文概要
---
所谓遍历二叉树，就是遵从某种次序，顺着某一条搜索路径访问二叉树中的各个结点，使得每个结点均被访问一次，而且仅被访问一次。本文详细介绍了二叉树的前序(又称先序)、中序和后序遍历的规则及其算法实现。本文全部代码示例可从{% asset_link BinTreeTraversal.cc 此处 %}获得。
<!--more-->

## 遍历的定义
---
"遍历"，即访问到二叉树中的所有结点，且每个结点仅被访问一次。“访问”的含义可以很广，如：输出结点的信息、修改结点的数据之等，但一般要求这种访问不破坏原来数据之间的逻辑结构。

本文中”访问“规定为输出当前遍历结点元素值，定义打印函数如下：
```cpp
template <class ElemType>
void Print(ElemType e)
{
    cout << e;
}
```

实际上，“遍历”是任何数据结构均有的公共操作，二叉树是非线性结构，每个结点最多有两个后继，则存在如何遍历，即按什么样的搜索路径遍历的问题。这样就必须规定遍历的规则，按此规则遍历二叉树，最后得到二叉树中所有结点组成的一个线性序列。

## 二叉树的遍历类型
---
根据二叉树的结构特征，可以有三类搜索路径：先上而下的按层次遍历、先左(子树)后右(子树)的遍历、先右(子树)后左(子树)的遍历。设访问根结点记作 $D$，遍历根左子树记作 $L$，遍历根的右子树记作 $R$，则可能的遍历次序有：$DLR、LDR、LRD、DRL、RDL、RLD$ 及层次遍历。若规定先左后右，则只剩下4种遍历方式：$DLR、LDR、LRD$ 及层次遍历，根据根结点被遍历的次序，通常称 $DLR、LDR和LRD$ 这3种遍历为前序遍历、中序遍历和后序遍历。

给出如下二叉树实例：
{% qnimg Algorithm/Algorithm/traversal-binary-tree/example-bintree.png %}

则其不同方式的遍历序列分别为：
- 层次遍历结果序列：ABCDEFG
- 前序遍历结果序列：ABDGCEF
- 中序遍历结果序列：DGBAECF
- 后序遍历结果序列：GDBEFCA

后文依次介绍了层次遍历以及前序、中序和后序算法实现，其中用到的两个二叉树相关的数据结构 [**BinTree**](https://github.com/sundongxu/data-structure/blob/master/BinTree.h) 和 [**BinTreeNode**](https://github.com/sundongxu/data-structure/blob/master/BinTreeNode.h) 可参见[**此篇**](dongdongdong.me)。

为简化二叉树遍历代码实现，给出以下二叉树的结点结构：
```cpp
template <class ElemType>
struct BinTreeNode
{
    ElemType val;                                                                // 结点关键码
    BinTreeNode *leftChild;                                                      // 左孩子结点
    BinTreeNode *rightChild;                                                     // 右孩子结点
    BinTreeNode(ElemType v) : val(v), leftChild(nullptr), rightChild(nullptr) {} // 构造函数
};
```

在调用本文中实现的遍历算法时，函数指针参数 `Visit` 即传入 `Print` 函数名即可。

## 层次遍历(Level-Order Traversal)
---
层次遍历是先访问层次小的所有结点，即从根结点开始，同一层次从左到右访问，然后再访问下一层次的结点。根据层次遍历的定义，除根结点外，每个结点都处于其双亲结点的下一层次，而指向每个结点的指针都记录在其双亲结点中，因此为了找到各结点，需将已经访问过的结点的孩子结点保存下来。使用一个 **队列** 来存储已访问过的结点的孩子结点。初始将根结点入栈，每次要访问的下一个结点都是队列上取出指向结点的指针，每访问完一个结点后，如果它有左孩子、右孩子结点，则将它的左、右孩子结点入队，如此重复，直到队列为空，则遍历结束。

下面给出二叉树**层次遍历**具体实现：
```cpp
template <class ElemType>
void LevelOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：层次遍历二叉树
    if (root == nullptr) // 二叉树为空，结束算法
        return;
    queue<BinTreeNode<ElemType> *> q;   // 辅助队列
    BinTreeNode<ElemType> *node = root; // 从根结点开始进行层次遍历
    q.push(node);
    while (!q.empty())
    {
        // 队列非空，说明还有结点未访问
        node = q.front();
        q.pop();
        (*Visit)(node->val);
        if (node->leftChild != nullptr)  // 左孩子非空
            q.push(node->leftChild);     // 左孩子入队
        if (node->rightChild != nullptr) // 右孩子非空
            q.push(node->rightChild);    // 右孩子入队
    }
}
```
## 前序遍历(Pre-Order Traversal)
---
二叉树的前序遍历定义如下：
如果二叉树为空，则算法结束。
否则：
1. 访问根结点(D)
2. 前序遍历左子树(L)
3. 前序遍历右子树(R)

前序遍历也称为先序遍历，就是按照“根-左子树-右子树”的次序遍历二叉树。

前序遍历算法分为递归和非递归实现。

### 递归遍历
---
```cpp
template <class ElemType>
void RecursionPreOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：前序遍历以root为根的二叉树(递归)
    if (root != nullptr)
    {
        (*Visit)(root->val);                       // 访问根结点
        PreOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        PreOrderTraverse(root->rightChild, Visit); // 递归访问右子树
    }
}
```

### 非递归遍历
---
```cpp
template <class ElemType>
void NonRecursionPreOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：前序遍历以root为根的二叉树(非递归)
    if (root == nullptr) // 二叉树为空，结束算法
        return;
    stack<BinTreeNode<ElemType> *> s; // 辅助栈
    BinTreeNode<ElemType> *p;         // 当前遍历结点指针
    s.push(root);
    while (!s.empty()) // 栈非空
    {
        p = s.top();
        s.pop();
        Visit(p->val); // 结点出栈即被访问
        if (p->rightChild != nullptr)
            s.push(p->rightChild);
        if (p->leftChild != nullptr)
            s.push(p->leftChild);
    }
}
```

## 中序遍历(In-Order Traversal)
---
二叉树的中序遍历定义如下：
如果二叉树为空，则算法结束。
否则：
1. 中序遍历左子树(L)
2. 访问根结点(D)
3. 中序遍历右子树(R)

中序遍历就是按照“左子树-根-右子树”的次序遍历二叉树。

中序遍历算法分为递归和非递归实现。

### 递归遍历
---
```cpp
template <class ElemType>
void RecursionInOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：中序遍历以r为根的二叉树
    if (root != nullptr)
    {
        InOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        (*Visit)(root->val);                      // 访问根结点
        InOrderTraverse(root->rightChild, Visit); // 递归访问右子树
    }
}
```

### 非递归遍历
---
```cpp
template <class ElemType>
void NonRecursionInOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：中序遍历以root为根的二叉树(非递归)
    if (root == nullptr) // 二叉树为空，结束算法
        return;
    stack<BinTreeNode<ElemType> *> s; // 辅助栈
    BinTreeNode<ElemType> *p = root;  // 当前遍历结点指针
    do
    {
        while (p != nullptr) // 遍历指针未到最左下结点，则不空
        {
            s.push(p);        // 该子树沿途结点进栈
            p = p->leftChild; // 遍历指针前进到左孩子结点
        }

        if (!s.empty()) // 栈不空时退栈
        {
            p = s.top();
            s.pop();
            Visit(p->val);     // 结点出栈即被访问
            p = p->rightChild; // 遍历指针前进到右孩子结点
        }

    } while (p != nullptr || !s.empty()); // p非空即本轮循环访问的结点还有右孩子 或 栈中还有结点
}
```

## 后序遍历(Post-Order Traversal)
---
二叉树的前序遍历定义如下：
如果二叉树为空，则算法结束。
否则：
1. 后序遍历左子树(L)
2. 后序遍历右子树(R)
3. 访问根结点(D)

后序遍历就是按照“左子树-右子树-根”的次序遍历二叉树。

后序遍历算法分为递归和非递归实现。

### 递归遍历
---
```cpp
template <class ElemType>
void RecursionPostOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：后序遍历以r为根的二叉树
    if (root != nullptr)
    {
        PostOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        PostOrderTraverse(root->rightChild, Visit); // 递归访问右子树
        (*Visit)(root->val);                        // 访问根结点
    }
}
```

### 非递归遍历
---
```cpp
enum Tag
{
    L,
    R
};

template <class ElemType>
struct StackNode
{
    // 在后序遍历非递归实现所用栈结点类定义
    BinTreeNode<ElemType> *ptr;                                       // 指向树结点的指针
    Tag tag;                                                          // 该结点的退栈标记
    StackNode(BinTreeNode<ElemType> *N = nullptr) : ptr(N), tag(L) {} // 构造函数
};

template <class ElemType>
void NonRecursionPostOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：后序遍历以root为根的二叉树(非递归)
    if (root == nullptr) // 二叉树为空，结束算法
        return;
    stack<StackNode<ElemType>> s; // 辅助栈
    StackNode<ElemType> w;
    BinTreeNode<ElemType> *p = root; // 当前遍历结点指针
    do
    {
        while (p != nullptr)
        {
            w.ptr = p;
            w.tag = L;
            s.push(w);
            p = p->leftChild;
        }
        bool shouldContinue = true;
        while (shouldContinue && !s.empty())
        {
            w = s.top();
            s.pop();
            p = w.ptr;
            switch (w.tag)
            {
            case L:
                w.tag = R;
                s.push(w);
                shouldContinue = false;
                p = p->rightChild;
                break;
            case R:
                Visit(p->val);
                break;
            }
        }
    } while (!s.empty());
}
```

## 参考资料
---
[1][数据结构(用面向对象方法与C++语言描述) - 殷人昆主编](https://www.amazon.cn/dp/B0011F7UHO)
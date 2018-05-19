#include <iostream>
#include <stack>
#include <queue>
using namespace std;

template <class ElemType>
struct BinTreeNode
{
    ElemType val;                                                                // 结点关键码
    BinTreeNode *leftChild;                                                      // 左孩子结点
    BinTreeNode *rightChild;                                                     // 右孩子结点
    BinTreeNode(ElemType v) : val(v), leftChild(nullptr), rightChild(nullptr) {} // 构造函数
};

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

template <class ElemType>
void RecursionPreOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：前序遍历以root为根的二叉树(递归)
    if (root != nullptr)
    {
        (*Visit)(root->val);                                // 访问根结点
        RecursionPreOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        RecursionPreOrderTraverse(root->rightChild, Visit); // 递归访问右子树
    }
}

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

template <class ElemType>
void RecursionInOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：中序遍历以r为根的二叉树
    if (root != nullptr)
    {
        RecursionInOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        (*Visit)(root->val);                               // 访问根结点
        RecursionInOrderTraverse(root->rightChild, Visit); // 递归访问右子树
    }
}

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

template <class ElemType>
void RecursionPostOrderTraverse(BinTreeNode<ElemType> *root, void (*Visit)(ElemType &))
{
    // 操作结果：后序遍历以r为根的二叉树
    if (root != nullptr)
    {
        RecursionPostOrderTraverse(root->leftChild, Visit);  // 递归访问左子树
        RecursionPostOrderTraverse(root->rightChild, Visit); // 递归访问右子树
        (*Visit)(root->val);                                 // 访问根结点
    }
}

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

template <class ElemType>
void Print(ElemType e)
{
    cout << e;
}

int main()
{
    BinTreeNode<char> A('A');
    BinTreeNode<char> B('B');
    BinTreeNode<char> C('C');
    BinTreeNode<char> D('D');
    BinTreeNode<char> E('E');
    BinTreeNode<char> F('F');
    BinTreeNode<char> G('G');

    A.leftChild = &B;
    A.rightChild = &C;
    B.leftChild = &D;
    D.rightChild = &G;
    C.leftChild = &E;
    C.rightChild = &F;

    cout << "层次遍历结果序列：";
    LevelOrderTraverse(&A, Print);
    cout << endl
         << endl;

    cout << "(递归)前序遍历结果序列：";
    RecursionPreOrderTraverse(&A, Print);
    cout << endl;
    cout << "(非递归)前序遍历结果序列：";
    NonRecursionPreOrderTraverse(&A, Print);
    cout << endl
         << endl;

    cout << "(递归)中序遍历结果序列：";
    RecursionInOrderTraverse(&A, Print);
    cout << endl;
    cout << "(非递归)中序遍历结果序列：";
    NonRecursionInOrderTraverse(&A, Print);
    cout << endl
         << endl;

    cout << "(递归)后序遍历结果序列：";
    RecursionPostOrderTraverse(&A, Print);
    cout << endl;
    cout << "(非递归)后序遍历结果序列：";
    NonRecursionPostOrderTraverse(&A, Print);
    cout << endl
         << endl;

    return 0;
}
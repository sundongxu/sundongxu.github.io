---
title: 数据结构之栈与队列(优先队列/堆)
date: 2018-05-03 10:10:10
categories:
- Algorithm
tags:
- Data Structure
- Stack
- Queue
- Priority Queue
- Heap
- C++
---

## 全文概要
---
**栈**与**队列**是两种重要的特殊线性表，从结构上讲，两者都是线性表，但从操作上讲，两者支持的基本操作却只是线性表操作的子集，是操作受限制的线性表。栈与队列两者最大的区别在于，栈元素后进先出(LIFO，Last In First Out)，而队列元素先进先出(FIFO，First In First Out)。此外，针对队列这一特殊数据结构，有时需考虑队列元素的优先级的关系，即根据用户自定义的优先级排序，出队时优先弹出优先级更高(低)的元素，**优先队列**能更好地满足实际问题中的需求，而在优先队列的各种实现中，**堆**是一种最高效的数据结构。本文分别介绍了**顺序栈**、**链式栈**、**链式队列**和**循环队列**以及对应与前两种队列实现的**最大/最小优先级队列**，还有两种堆结构，**最大堆**与**最小堆**的基本结构，并给出了相应的C++类代码实现。
<!--more-->

## 栈(Stack)
---
栈是限定只能在表头进行插入(入栈)与删除(出栈)操作的线性表，表头端称为栈顶，表尾端称为栈底。

设有栈 $S=(a_1, a_2,..., a_n)$ ，则一般称 $a_1$ 为栈底元素，$a_n$为栈顶元素，按 $a_1, a_2,...,a_n$ 的顺序依次进栈，则根据元素入栈的规定，从栈顶到栈底的元素依次为 $a_n,...,a_2, a_1$，出栈时弹出的第一个元素为栈顶元素，即 $a_n$，也就是说栈是按**后进先出**的原则进行。故，栈可称为**后进先出(Last In First Out, LIFO)**的线性表。

栈有顺序表和链表两种实现方式。

### 顺序栈(Sequence Stack)
---
在顺序实现中，利用一组地址连续的存储单元即数组依次存放从栈底到栈顶的数据元素，将数据类型为`ElemType`的数据元素存放在数组中，并用`count`存储数组中存储的栈的实际元素个数。每次插入新的栈顶元素，如栈未满，则操作成功，`count`值加一，而当删除栈顶元素时，如栈不空，操作成功，并且`count`值减一。

顺序栈的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**SqStack.h**](https://github.com/sundongxu/data-structure/blob/master/SqStack.h)|[**SqStack.cc**](https://github.com/sundongxu/data-structure/blob/master/SqStack.cc)|

### 链式栈(Link Stack)
---
在程序中同时使用多个栈的情况下，使用链式栈不仅可以提高存储效率，同时还可以达到共享存储空间的目的。链表实现的栈，入栈和出栈都非常简单，一般也都不使用头结点直接实现。

链式栈的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**LinkStack.h**](https://github.com/sundongxu/data-structure/blob/master/LinkStack.h)|[**LinkStack.cc**](https://github.com/sundongxu/data-structure/blob/master/LinkStack.cc)|

## 队列(Queue)
---
队列是一种先进先出(First In First Out，FIFO)的线性表，只允许在一端进行插入(入队)操作，而在另一端进行删除(出队)操作。

在队列中，允许入队操作的一端称为队尾，允许出队操作的一端称为队头。

设有队列 $q=(a_1, a_2,...,a_n)$，则 $a_1$ 称为队头元素，$a_n$ 称为队尾元素，队列中元素是按 $a_1, a_2,...,a_n$ 的顺序入队，同时也要求按照相同的顺序出队。

队列也有两种存储结构：顺序存储结构和链式存储结构。

### 链队列(Link Queue)
---
用链表表示的队列称为链队列，一个链队列应用两个分别指示队头与队尾的指针，分别称为头指针(`front`)和尾指针(`rear`)。其中头指针指向该队列的头结点，即不包含实际数据域的结点，而尾指针指向队列最后一个结点，是包含实际数据域的结点。

如果要从队列中弹出一个元素，必须从单链表的第一个结点中取出队头元素，并删除此节点，而入队的新元素是存放在队尾处的，也就是单链表的最后一个元素的后面，并且此结点将成为新的队尾。

链队列适合于数据元素个数变动比较大的情况，一般不存在溢出的问题，如果程序中要使用多个队列，最好使用链队列，这样将不会出现存储分配的问题，也不必进行数据元素的移动。

链队列的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**LinkQueue.h**](https://github.com/sundongxu/data-structure/blob/master/LinkQueue.h)|[**LinkQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/LinkQueue.cc)|

### 循环队列(Circular Queue)
---
如果用C++描述队列的顺序存储结构，实际是利用一个一位数组`elem`作为队列的元素存储结构，并分别设立了两个指针`front`和`rear`分别表示队头和队尾，`maxSize`是队列的最大元素个数。

但这样容易导致一个问题，试想，假设新创建一个队列，最大空间`maxSize`为5，那么依次入队5个元素，此时`front`依然指向队头，即数组第一个位置之前的位置，`rear`则指向队尾，即数组最后一个位置，然后出队2个元素，即从队头开始弹出2个元素，即删除数组`elem`前两个元素，此时队头指针`front`后移至数组第2个位置(有效元素从第3个位置，`front`永远指向第1个有效元素所在位置之前的一个位置)。若此时想再入队一个新元素，即从队尾插入新元素，但是由于队尾指针`rear`已经到达数组最后一个位置，不能再后移，即不允许再插入元素，而数组前2个位置由于之前的两次出队操作空出了2个位置，即此时队列的实际可用空间还没有使用完，这种情况下再插入一个新元素产生的溢出称为**“假溢出”**。

解决**假溢出**的一个较巧妙的方法是将顺序队列从逻辑上看成一个环，成为一个循环队列，循环队列的首尾相接，当队头`front`和队尾`rear`进入到`maxSize-1`时，再进一个位置就自动移动到0，可用取余运算(%)简单地实现：

> 队头前进1个位置：`front = (front + 1) % maxSize`
> 队尾前进1个位置：`rear = (rear + 1) % maxSize`

但仅仅这样处理带来一个问题，不难发现当队列为满时，有  `front == rear`，而当队列为空时，依然有 `front == rear`。
可知，仅从`front == rear`这一条件并不能判断究竟是队空还是队满，那我们也有两种处理方法：
1. 另设一个标志位区别是队空还是队满
2. 少用一个元素空间，约定队头在队尾指针的下一个位置时作为队满的标志

本文采用第2种，即浪费一个元素空间方法，那么在这种情况下：

> 队空时，有`front == rear`
> 队满时，有`(rear - front + maxSize) % maxSize == maxSize - 1`

而队列中的元素个数即为 `(rear - front + maxSize) % maxSize`，在牺牲一个位置不存储元素的情况下，若元素个数 `== maxSize - 1`，当然队列也就是满的。 

循环队列的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**CircQueue.h**](https://github.com/sundongxu/data-structure/blob/master/CircQueue.h)|[**CircQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/CircQueue.cc)|

## 优先级队列(Priority Queue)
在许多情况下，前面介绍的普通队列是不够用的，先进先出的机制有时需要某些优先规则来完善使其更能适用于实际场景。比如在医院中，病危患者应具有更高的优先级，若还是按先来后到顺序对排队患者依次治疗，显然是不合理的，也就是说，当医生有空时，应立刻从患者中选择病情最危急者优先救治，此处的患者的病情危重程度就决定了其就诊的优先级。

一般地，优先级高低实际就决定了队列中元素的出队顺序。

优先队列是一种基于队列并同时考虑了优先级的数据结构，其中元素的固有顺序决定了对基本操作的执行结果，优先队列有两种类型：最小优先队列和最大优先队列。最小优先队列的出队操作`OutQueue()`将删除最小的数据元素值，最大优先队列的出队操作`OutQueue()`将删除最大的数据元素值。

优先队列有多种实现方法，一种比较简单的实现方法是在作入队操作`InQueue()`时，元素不是排在队列的末尾，而是根据其优先级将其插入到队列中的合适位置，使队列的元素有序，优先队列类可作为队列类的派生类来实现，只需覆盖队列类的入队操作`InQueue()`即可。

基于链队列和循环队列，每种队列又可派生出最大、最小两种优先队列，共计四种优先队列的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**MaxPriorityLinkQueue.h**](https://github.com/sundongxu/data-structure/blob/master/MaxPriorityLinkQueue.h)|[**MaxPriorityLinkQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/MaxPriorityLinkQueue.cc)|
|[**MinPriorityLinkQueue.h**](https://github.com/sundongxu/data-structure/blob/master/MinPriorityLinkQueue.h)|[**MinPriorityLinkQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/MinPriorityLinkQueue.cc)|
|[**MaxPriorityCircQueue.h**](https://github.com/sundongxu/data-structure/blob/master/MaxPriorityCircQueue.h)|[**MaxPriorityCircQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/MaxPriorityCircQueue.cc)|
|[**MinPriorityCircQueue.h**](https://github.com/sundongxu/data-structure/blob/master/MinPriorityCircQueue.h)|[**MinPriorityCircQueue.cc**](https://github.com/sundongxu/data-structure/blob/master/MinPriorityCircQueue.cc)|

## 堆(Heap)
---
在上节介绍优先队列时提到，构造优先队列的方法是通过简单地在普通队列将新元素入队时，为其按优先级高低(元素值大小)找到合适的位置再插入，而不是直接插入在队尾，这种方式得到的优先队列的元素是严格有序排列的，如最大优先队列中，元素从大到小排列，最大元素即队头元素。这种优先队列的实现，思想简单，但同时开销也比较大，效率不够高，本节会介绍一种叫做堆的数据结构，在优先队列的各种实现中，堆是最高效的一种数据结构。

不难想象，若数据集合严格有序，将为各种操作带来遍历，如上节的优先队列的实现，但是，有些场景并不要求数据全部有序，或者在操作开始前就完全有序。在许多应用中，通常需要收集一部分数据，从中挑选具有最小或最大关键码(优先级)的记录开始处理。接着，可能会收集更多数据，并处理当前数据集中具有最小或最大关键码的记录。对于此类应用，我们期望的数据结构，应能支持插入操作，并能方便地从中取出具有最小或最大关键码的记录，这样的数据结构其实就是优先队列。也就是说，优先队列仅仅要求能够方便地找到数据中关键码最小或最大，即优先级最低或最高的记录，其实并不要求数据严格排好序，并能保证出队时总能找到关键码最小或最大的记录优先出队，堆正好可以满足这一需求，而堆是局部有序的，并非严格有序。

假定在各个数据记录或元素中，存在一个能够标识数据记录或元素的数据项，并将依据该数据项对数据进行组织，则可称此数据项为关键码(key)。

如果有一个关键码的集合，把它的所有元素按**完全二叉树**的顺序存储方式存放在一个一位数组中。如果有：
>每个父结点的关键码总比其孩子结点的关键码大
于是位于堆顶(即二叉树根结点)的结点的关键码最大，故称该集合为**最大堆**

或者：

>每个父结点的关键码总比其孩子结点的关键码小
于是位于堆顶(即二叉树根结点)的结点的关键码最小，故称该集合为**最小堆**

在堆中，所有的结点具有称之为**“堆序列”**的关系，同样，“堆序”分为最小堆序和最大堆序。具有最小堆序的结点之间存在小于或等于关系，具有最大堆序的结点之间存在大于或等于的关系。

根据**完全二叉树**的性质，由堆存储在下标为0开始计数的数组中，因此，在堆(数组)中给定下标为 $i$的结点时：
1. 如 $i=0$，则结点 $i$ 为根结点，无父结点，否则结点 $i$ 的父结点为结点 $\lfloor\frac{i-1}{2}\rfloor$
2. 如 $2i+1>n-1$，则结点 $i$ 无左子女，否则结点 $i$ 的左子女为结点 $2i+1$
3. 如 $2i+2>n-1$，则结点 $i$ 无右子女，否则结点 $i$ 的右子女为结点 $2i+2$

另外还会用到的一个性质：
- 完全二叉树最后一个非叶结点的下标为 $\lfloor\frac{n-2}{2}\rfloor$

下节会以**最小堆**为例讲解堆的构造和调整过程。

### 最小堆
---
父结点的关键码总是小于其孩子结点的完全二叉树称为最小堆。

#### 堆的构造
当给出一个记录的关键码集合时，首先把它的记录顺序放在堆的`Heap`数组中，最初数据的排列显示它不是一个最小堆，因此需要把它调整成为一个堆。

我们采用从下向上逐步调整形成堆的方法：轮流以完全二叉树结点编号从 $\lfloor\frac{n-2}{2}\rfloor$ ，即完全二叉树最后一个非叶结点开始，一直到编号为0，即根结点，调用下滑调整算法`SiftDown()`，将以它们为根的子树调整成为最小堆，从局部到整体，将最小堆逐步扩大，直到在根结点处也调整完成后，整个树即被调整成为最小堆。

`SiftDown()`是一个自上而下的调整算法，其基本思想是：
1. 对有 $m$ 个记录的集合 $R$，将它置为完全二叉树的顺序存储。首先从结点 $i$ 开始向下调整，即把以它为根结点的子树调整成为最小堆，前提条件是假定它的两棵子树都已成为最小堆(所以一开始要从完全二叉树的第一个非叶结点开始整个调整过程，因此非叶结点的子树即叶子结点，单个结点本身就是最小堆)。
2. 如果结点 $i$ 的左孩子的关键码小于右孩子的关键码：$R[j].key < R[j+1].key$，其中 $j=2i+1，即j是i的左孩子，j+1是i的右孩子$，则按结点 $i$ 的左分支进行调整，否则沿右分支调整。让 $j$ 指向参加调整的孩子结点，调整的方法是以 $R[i] 与 R[j]$ 进行关键码的比较：若 $R[i].key > R[j].key$，则把关键码小的孩子 $j$ 上浮到 $i$ 的位置。
3. 然后 $i和j$ 进一步下沉，即令 $i=j, j=2j+1$，继续向下一层进行比较；若 $R[i].key \leq R[j].key$，则不对调，也不再向下一层继续比较，因为此时可判定以 $i$ 为根结点的子树已经是最小堆了，算法终止。
4. 最后结果是关键码最小的结点上浮到了堆顶，且完全二叉树的每个局部都是一个最小堆，符合最小堆定义，最小堆形成。

给出`SiftDown()`代码实现如下：
```cpp
template <class ElemType>
void MinHeap<ElemType>::siftDown(int start, int m)
{
    // 从结点start开始到结点m为止，自上向下比较，如果子女的值小于父结点的值
    // 则关键码更小的子女上浮，继续向下层比较，这样将一个集合局部调整为最小堆
    int i = start, j = 2 * i + 1; // j是i的左孩子
    ElemType temp = heap[i];
    while (j <= m)
    {
        if (j < m && heap[j] > heap[j + 1]) // i的右孩子比j(i的左孩子)更小，j指向右孩子(更小的)
            j++;
        if (temp <= heap[j]) // i已经比其子女j小了，调整结束
            break;
        else
        {
            heap[i] = heap[j]; // i比子女j大，则j上浮替代i的位置，调整位置i和j均下降
            i = j;
            j = 2 * j + 1;
        }
    }
    heap[i] = temp; // 放回start结点
}
```

以上代码中值得一提的一个细节是：关键码比较时用的是比较运算符**">"**和**"<="**，这些运算符应该在元素类型`ElemType`中重载定义。如果堆中元素各不相同的话，其实可以只需">"便足够，将`temp <= heap[j]`改为`heap[j] > temp`即可，此运算符应该在元素类型`ElemType`中重载定义。从而可以总结如下规律：
> 在元素互异的理想情况下，实现最小堆要求元素类型重载**“<“**运算符，而实现最大堆则要求元素类型重载**”>“**运算符。

给出堆的构造函数`MinHeap()`代码实现如下：
```cpp
template <class ElemType>
MinHeap<ElemType>::MinHeap(ElemType arr[], int n)
{
    maxHeapSize = (DEFAULT_SIZE < n) ? n : DEFAULT_SIZE;
    heap = new ElemType[maxHeapSize];
    if (heap == nullptr)
    {
        cerr << "heap create failed!" << endl;
        exit(1);
    }
    for (int pos = 0; pos < n; pos++)
        heap[pos] = arr[pos];
    currentSize = n;
    int currentPos = (currentSize - 2) / 2; // 找最初调整位置：最后分支结点(最后一个有孩子的结点)
    while (currentPos >= 0)                 // 自底向上逐步调整形成堆
    {
        siftDown(currentPos, currentSize - 1); // 局部自上向下下滑调整
        currentPos--;                          // 再向前换一个分支结点
    }
}
```

#### 堆的插入
最小堆的插入算法则调用了另一种的调整算法`SiftUp()`，实现自下而上的上滑调整。因为每次新结点总是插在已经建成的最小堆的后面，这是必须遵循与`SiftDown()`相反的比较路径，从下而上，与父结点的关键码进行比较、对调。

给出`SiftUp()`代码实现如下：
```cpp
template <class ElemType>
void MinHeap<ElemType>::siftUp(int start)
{
    // 从结点start开始到结点0为止，自下而上比较，如果子女的值小于父结点的值
    // 则子女上浮，继续向上层比较，这样将一个集合重新调整为最小堆，元素类型ElemType应自行定义"<="运算符
    int j = start, i = (j - 1) / 2;
    ElemType temp = heap[j];
    while (j > 0) // 沿父结点路径向上直达根，i是j的父结点
    {
        if (heap[i] <= temp) // 父结点值小，无需调整
            break;
        else
        {
            heap[j] = heap[i]; // i比子女j大，则i下滑取代j的位置，调整位置i和j均上滑
            j = i;
            i = (i - 1) / 2;
        }
    }
    heap[j] = temp;
}
```

同样地，`SiftUp()`中用到了比较运算符**"<="**，它应该在元素类型`ElemType`中重载定义。

给出`Insert()`代码实现如下：
```cpp
template <class ElemType>
bool MinHeap<ElemType>::Insert(const ElemType &e)
{
    if (currentSize == maxHeapSize) // 堆满
    {
        cerr << "Heap already full!" << endl;
        return false;
    }
    heap[currentSize] = e; // 每次新元素总会插入到堆末尾
    siftUp(currentSize);   // 从当前元素即尾元素开始向上调整
    currentSize++;
    return true;
}
```

#### 堆的删除
通常，从最小堆中删除具有最小关键码记录的操作是将最小堆的堆顶元素，即其对应完全二叉树的顺序表示的第0号元素删去。把这个元素取走后，一般以堆的最后一个结点填补取走的堆顶元素，并将堆的实际元素个数减1。但是用最后一个元素取代堆顶元素将破坏堆，需要调用`SiftDown()`算法从堆顶向下调整。

给出`RemoveMin()`代码实现如下：
```cpp

template <class ElemType>
bool MinHeap<ElemType>::RemoveMin(ElemType &e)
{
    if (currentSize == 0) // 堆空
    {
        cerr << "Heap already empty!" << endl;
        return false;
    }
    e = heap[0];
    heap[0] = heap[currentSize - 1]; // 最后元素填补到根结点
    currentSize--;
    siftDown(0, currentSize - 1); // 因为尾元素放到了堆顶，故从堆顶元素开始自上而下调整
    return true;
}
```

从完全二叉树的性质可知，$n$ 个结点的完全二叉树的深度为 $k=\lceil\log{(n+1)}\rceil$，应用堆的调整算法`SiftDown()`时，`while`循环次数最大为树的深度减1，所以堆的删除算法的时间复杂度为 $O(\log{n})$。而在插入一个新结点时，使用了一个堆的上滑调整算法`SiftUp()`，其中`while`循环次数不超过树的深度减1，所以堆的插入算法的时间复杂度也是 $O(\log{n})$。建树操作执行了 $\lfloor\frac{n}{2}\rfloor$ 次`SiftDown()`算法，其时间复杂度为 $O(n\log{n})$。

最小堆的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**MinHeap.h**](https://github.com/sundongxu/data-structure/blob/master/MinHeap.h)|[**MinHeap.cc**](https://github.com/sundongxu/data-structure/blob/master/MinHeap.cc)|

### 最大堆
---
父结点的关键码总是大于其孩子结点的完全二叉树称为最大堆。

最大堆的类声明及定义如下：

|  Header  |  Implementation  |
| :---: | :---: |
|[**MaxHeap.h**](https://github.com/sundongxu/data-structure/blob/master/MaxHeap.h)|[**MaxHeap.cc**](https://github.com/sundongxu/data-structure/blob/master/MaxHeap.cc)|

## 参考资料
---
[1][数据结构与算法(C++版) - 唐宁九主编](https://www.amazon.cn/dp/B001TREOXM)
[2][数据结构(用面向对象方法与C++语言描述) - 殷人昆主编](https://www.amazon.cn/dp/B0011F7UHO)
---
title: 常见算法之排序
date: 2018-04-23 12:12:12
categories:
- Algorithm
tags:
- Algorithm
- Sort
---

## 全文概要
各类排序算法，不仅是算法基本功，也是面试中永恒的考题，关于每种算法**思想**、**实现**(递归与非递归)以及**时空复杂度分析**是必须牢牢把握的送分题。本文先将排序算法按不同标准进行分类，随后依次详细介绍了直接插入、希尔、冒泡、快速、简单选择、堆、归并、基数与外部排序等经典排序算法，并在文末给出了各种排序方法的性能比较作为总结。本文全部代码实例可从{% asset_link Sort.cc 此处 %}获得。
<!--more-->

排序(Sorting)，就是将数据元素(或记录)的任意序列，重新排序成按关键字有序的序列，下文介绍的排序算法实现均针对于整型数组，且最终结果序列按元素**从小到大(即升序)**排列。

## 排序分类

按照排序过程中所涉及的存储器，可将排序分为如下两类：
- **内部**排序：待排序的数据元素全部存入计算机内存中，在排序过程中无需访问外存(如磁盘)

- **外部**排序：待排序的数据元素不能全部装入内存(如数据量过大)，在排序过程中需要不断访问外存

本文仅介绍内部排序的几种经典算法，外部排序则暂不考虑。

对于内部排序，按排序过程中所依据的思想，又可分为如下四类：
1. **插入**排序
2. **交换**排序
3. **选择**排序
4. **归并**排序

按内排序过程中所需工作量(时间复杂度)，还可分为如下三类：
1. 简单排序方法，对应时间复杂度为**$O(n^2)$**
2. 先进排序方法，对应时间复杂度为**$O(n\log{n})$**
3. 基数排序方法，对应时间复杂度为**$O(dn)$**

而根据序列中原本有序的元素的相对位置关系是否在排序前后发生改变，又可分为如下两类：
- **稳定**排序：在原序列中有两个元素 $a=b$，且 $a$ 位于 $b$ 之前，若在排序后的新序列中 $a$ 仍然位于 $b$ 之前，则称该排序算法是稳定的。稳定的排序算法有：直接插入排序、

- **不稳定**排序：在原序列中有两个元素$a=b$，且 $a$ 位于 $b$ 之前，若在排序后的新序列中 $a$ 不一定位于 $b$ 之前，则称该排序算法是不稳定的。不稳定的排序算法有：

## 性能评估
排序算法的执行时间是衡量算法好坏的最重要参数。排序的时间开销可用算法执行中的**数据比较次数**和**数据移动次数**来衡量。一般排序算法运行时间代价的估算都按平均情况(Average Case)进行估算，对于那些受元素初始排列和元素个数影响较大的，则还需要按最好情况(Best Case)和最差情况(Worst Case)。

本文介绍的排序算法中，简单排序算法，如直接插入排序、冒泡排序和选择排序的(平均)时间开销(复杂度)均为 $O(n^2)$。而更为高效的排序方法，如快速排序、归并排序和堆排序算法，(平均)时间开销(复杂度)均为 $O(n\log{n})$，在本文中，若无特殊说明，则$\log$均表示以2为底的对数。

## 插入排序(Insertion Sort)
---
### 直接插入排序(Straight Insertion Sort)
---
#### 基本思想
初始化结果序列里面只有第一个数据元素，将第一个数据元素看成一个有序子序列，再以此从第二个数据元素起逐个与结果序列这个有序序列比较，通过比较为当前元素找到合适的位置进行插入，直到插入最后一个数据元素后，整个结果序列数组即有序。

一般地，在第 $i$ 步上，将 $elem[i]$ 插入到由 $elem[0]$ ~ $elem[i-1]$构成的有序子序列中。

#### 代码实现
```cpp
void StraightInsertSort(vector<int> &elem, int n)
{
    for(int i = 1; i < n; i++)
    {
        // 进行第i次插入，待插入元素为elem[i]
        // 前面0~i-1都已有序
        int e = elem[i];  // 暂存待插入元素
        int j; 
        for(j = i - 1; j >= 0 && e < elem[j]; j--)
        {
            // 将比elem[i]大的都后移一位
            elem[j+1] = elem[j]; 
        }
        // 结束上述循环即找到令elem[i] >= elem[j]的位置j
        // 此时elem[i]应插入在elem[j]之后一个位置
        elem[j+1] = e;
    }
}
```

#### 复杂度分析
插入排序算法由嵌套的两个 $for$ 循环组成，外层循环执行 $n-1$ 次，内层循环比较复杂，循环次数依赖于第 $i$ 个元素前面的元素值比 $elem[i]$ 大的元素个数。

**最坏情况下**，每个元素都必须移动到数组的最前面，如果原数组是逆序(即降序)的话就会出现此种情况，此时第一趟循环1次，第二趟循环2次，以此类推，第 $i$ 趟循环 $i$次，第 $n-1$ 趟循环 $n-1$ 次，故总比较次数$=1+2+3+...+(n-1)=\frac{n(n-1)}{2}$，即时间复杂度为 $O(n^2)$。

**最好情况下**，即原数组已经递增有序，这是每个内层 $for$ 循环刚进入即退出，只比较一次，而不移动元素，总的比较次数就是外循环次数 $n-1$ 次，可知此时时间复杂度为 $O(n)$。

另外，如待排序数组元素的顺序是随机的，也就是排序的元素可能出现的各种排序的概率是相同的，可以证明直接插入排序在此**平均情况下**，时间复杂度为 $O(n^2)$。

从以上讨论可知，直接插入排序的运行时间与待排序元素的初始排列顺序密切相关。

**直接插入排序是一种稳定的排序算法。**

### 折半插入排序(Binary Insertion Sort)
---
#### 基本思想
初始化结果序列里面只有第一个数据元素，将第一个数据元素看成一个有序子序列，每次插入一个元素时使用折半搜索法为其找到合适的插入位置，直到插入最后一个数据元素后，整个结果序列数组即有序。

#### 代码实现
```cpp
void BinaryInsertSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // 进行第i次插入，待插入元素为elem[i]
        // 前面0~i-1都已有序
        int e = elem[i];           // 暂存待插入元素
        int low = 0, high = i - 1; // 范围端点
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (e > elem[mid])
                low = mid + 1;
            else // e <= elem[mid]
                high = mid - 1;
        }
        // 结束上述循环即找到elem[i]应插入的位置为low
        for (int j = i - 1; j >= low; j--)
            elem[j + 1] = elem[j];

        elem[low] = e;
    }
}
```

#### 复杂度分析
折半搜索比顺序搜索快，所以折半插入排序就平均性能来说肯定比直接插入排序要好。它所需要的元素比较次数与待排序元素序列的初始排列无关，而仅仅依赖于元素个数。在插入第 $i$ 个元素时，需要经过 $\lfloor\log{i}\rfloor+1$次元素比较，才能确定它应插入的位置。

当 $n$ 较大时，总元素的比较次数比直接插入排序的最差情况要好得多，但比其最好情况要差。所以，在元素初始排列已经有序或接近有序时，直接插入排序比折半插入排序所需执行的元素比较次数要少。而两者的元素移动次数相同，依赖于元素的初始排列。

折半插入排序改变了插入排序算法的比较次数，未改变其待排序数据移动次数。最好情况下，插入的位置，刚好是二分位置，时间复杂度为 $O(n\log{n})$。最坏情况下，时间复杂度为 $O(n^2)$。平均时间复杂度为 $O(n^2)$。

**折半插入排序是一个稳定的排序方法。**

### 希尔排序(Shell Sort)
---
从上面对直接插入排序算法的分析，在最坏情况下其时间复杂度为 $O(n^2)$，在最好情况下其时间复杂度为 $O(n)$，不难推断出：
>① 如果**待排序元素基本有序**的话，应用直接插入排序的效率将大大提高。

而显然：
>②当**元素个数n较小时**，直接插入排序的效率也较高。

Shell排序正是从这两方面出发对插入排序进行改进而得到的一种效率较高的排序算法。

#### 基本思想
先将整个待排数据元素序列分割成若干子序列，分别对个子序列进行直接插入排序，等整个序列中的数据元素“基本有序”时，再对全体数据元素进行一次直接插入排序。

设待排序元素序列有 $n$ 个元素，首先取一个整数 $incr<n$ 作为间隔，将全部元素分成 $incr$ 个子序列(位置 0 ~ $incr-1$的元素分属不同子序列，故有 $incr$ 个子序列)，所有距离为 $incr$ 的元素放在同一个子序列中，在每一个子序列中分别施行直接插入排序。然后缩小间隔 $incr$，例如 $incr=\frac{incr}{2}$，重复上述子序列划分和排序工作，直至 $incr=1$，即将所有元素都划分至同一个序列中再排序完成为止。

由于开始时 $incr$ 的取值较大，**每个子序列中的元素较少**，排序速度较快(因为①)；而待到排序的后期，$incr$ 取值不断递减，子序列的元素数量逐渐增多，但由于前面工作的基础，**大多数元素已基本有序**，所以排序速度依然很快(因为②)。

#### 代码实现
```cpp
void ShellInsert(vector<int> &elem, int n, int incr)
{
    // 对数组elem作一趟增量为incr的Shell排序
    // 与插入排序的区别在于此处子序列中前后相邻记录的增量为incr，而不是1
    for (int i = incr; i < n; i++)
    {
        // 第i趟插入排序
        int e = elem[i]; // 暂存待插入元素
        int j;
        for (j = i - incr; j >= 0 && e < elem[j]; j -= incr)
        {
            // 将子序列中比e大的记录都后移，注意是在子序列中后移，下一个位置是j+incr
            elem[j + incr] = elem[j];
        }
        // 结束上述循环即找到令elem[i] >= elem[j]的位置j
        // 此时elem[i]应该放在elem[j]在同一个子序列中的下一个位置，即j+incr
        elem[j + incr] = e;
    }
}

// t为增量序列长度
void ShellSort(vector<int> &elem, int n, vector<int> &incrs, int t)
{
    for (int k = 0; k < t; k++)
    {
        // 第k趟希尔排序
        ShellInsert(elem, n, incrs[k]);
    }
}
```

#### 复杂度分析
分析希尔排序是一个十分复杂的问题，它的时间复杂度是“增量”序列的函数，到现在为止尚未得到数学上的解决。有人利用大量的实验统计资料得出，当 $n$ 很大时，元素的平均比较次数和平均移动次数大于在 $n^{1.25}$ 到 $1.6n^{1.25}$ 的范围内，这是在利用直接插入排序作为子序列排序方法的情况下得到的。还有人指出，当增量序列第 $k$ 个元素 $incrs[k]=2^{t-k+1}-1$ 时，希尔排序的平均时间复杂度为 $O(n^{1.5})$。

至于增量序列 $incrs$ 应如何选取，则有多种方案。最初Shell提出取 $incr=\frac{n}{2}$，$incr=\frac{incr}{2}$，直到 $incr=1$。但由于直到最后一步奇数位置上的元素才会和偶数位置上的元素进行比较，这样使用这个序列的效率将很低。后来Knuth提出取 $incr=\frac{incr}{3}+1$。还有人提出都取奇数为好，也有人提出各个增量 $incr$ 互质更好。应用不同的增量会对希尔排序算法的性能有很大影响，有些序列的效率会有明显的提高。

**希尔排序是一种不稳定的排序算法。**

## 交换排序(Exchange Sort)
---
### 冒泡排序(Bubble Sort)
---
由于在该排序算法对应的过程中，较小的元素向水中的气泡一样逐渐向上(前)漂浮，较大的元素像石块一样向下(后)沉，并且每一趟比较完成后都有一块最大的“石块”沉到水底，冒泡排序因此得名。

#### 基本思想
将序列的第1个元素与第2个元素进行比较，如前者大于后者，则两个元素交换位置，否则不交换；再将第2个元素与第3个元素进行比较，根据对应大小关系决定是否交换位置...以此类推，直到第 $n-1$ 个元素和第 $n$ 个元素比较(或交换或不交换)。经过如此一趟排序，使得 $n$ 个元素中的最大者被安置在第 $n$ 个位置上(即它的正确位置)。此后，再对前 $n-1$ 个元素进行同样过程，使得该 $n-1$ 个元素的最大者被安置在第 $n-1$ 个位置上(也即它的正确位置)。以此类推，直到某一趟排序过程不出现元素位置交换的动作，即完成第 $n-1$ 趟排序，排序过程结束。

#### 代码实现
```cpp
void BubbleSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // 第i趟冒泡排序，将第i大的元素放在位置n-i上
        for (int j = 0; j < n - i; j++)
        {
            // 在第i趟冒泡排序过程中需要执行n-i次比较和交换操作
            if (elem[j] > elem[j + 1])
                swap(elem[j], elem[j + 1]);
        }
    }
}
```

#### 算法改进
考虑到具体的一个待排元素序列时可能不需要 $n-1$ 趟冒泡排序就已经排好序了，增加一个标志位 `exchange` 用于标识此趟($n-1$ 次外循环中的某一次)排序是否发生了逆序和交换。如果没有交换则 `exchange` 始终为`false`，表示全部元素已经排好序了，从而可以提前终止处理，结束算法，而无需非要完成全部 $n-1$ 趟排序。如果 `exchange` 为 `true`，则表示本趟排序有元素交换发生，还需执行下一趟排序。

改进后的冒泡排序代码实现如下：
```cpp
void AdvancedBubbleSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // 第i趟冒泡排序，将第i大的元素放在位置n-i上
        bool exchange = false;
        for (int j = 0; j < n - i; j++)
        {
            // 在第i趟冒泡排序过程中需要执行n-i次比较和交换操作
            if (elem[j] > elem[j + 1])
            {
                swap(elem[j], elem[j + 1]);
                exchange = true;  // 本趟排序发生交换
            }
        }
        if (!exchange) // 本趟排序未发生交换，则排序完成，算法结束
            return;
    }
}
```

#### 复杂度分析
冒泡排序的外层for循环次数为 $n-1$，内层for的循环次数为 $i$，可知内层循环的元素总比较次数为：
<center>$$1+2+...+(n-1)=\frac{n(n-1)}{2}=O(n^2)$$</center>

冒泡排序中，第 $i$ 趟排序需要执行 $n-i$ 次元素比较操作，但不是每次比较都会导致交换(移动)操作。即冒泡排序的数据比较次数与初始序列排列顺序无关，均需要 $\frac{n(n-1)}{2}$ 即 $O(n^2)$ 次操作，但数据交换次数与各待排序元素的初始排列有关，它与逆序的发生有关，最好情况下可能一次都不交换，最差情况下每一次比较操作都会导致元素交换即移动操作，但时间复杂度都是 $O(n^2)$。另外还可证明在平均情况下，冒泡排序的时间复杂度仍为 $O(n^2)$。

**冒泡排序是一种稳定的排序算法。**

### 快速排序(Quick Sort)
---
快速排序平均时间性能最快，有着广泛应用，实际上标准C++类库即STL中的排序程序就被称为 `qsort` (当然还有 `sort` )，即默认使用快速排序实现。但值得注意的是，在初始序列有序的情况下，快速排序的时间性能反而最差。

#### 基本思想
任选序列中的一个数据元素，通常选第一个元素，作为**枢轴(pivot)**，用它和所有剩余数据进行比较，将所有比它小的数据元素都排在它之前，将所有比它大的数据元素都排在它之后，经过一趟排序后，可按此数据元素即枢轴元素位置为界，将序列划分为两个部分，并且该枢轴元素已经放在了它应该放置的位置。之后再对这两个部分重复上述步骤，即递归，直至每一个部分中只剩下一个数据元素为止。

#### 代码实现
基于上节介绍的算法思想，有如下快排的递归实现：
```cpp
int Partition(vector<int> &elem, int low, int high)
{
    // 初始枢轴元素为elem[low]
    // 交换elem中的元素，使枢轴移动到适当位置
    // 要求在枢轴之前的元素不大于枢轴，在枢轴之后的元素不小于枢轴
    // 并返回枢轴位置
    while (low < high)
    {
        while (low < high && elem[low] <= elem[high])  // elem[low]为枢轴
            high--;
        swap(elem[low], elem[high]);
        while (low < high && elem[high] >= elem[low])  // elem[high]为枢轴
            low++;
        swap(elem[low], elem[high]);
    }
    return low;  // 返回枢轴位置
}

void QuickSortHelp(vector<int> &elem, int low, int high)
{
    if (low < high)  // 有效序列边界
    {
        int pivot = Partition(elem, low, high);
        QuickSortHelp(elem, low, pivot - 1);  // 递归，对子数组elem[low, pivot-1]进行排序
        QuickSortHelp(elem, pivot + 1, high); // 递归，对子数组elem[pivot+1, high]进行排序
    }
}

void RecursionQuickSort(vector<int> &elem, int n)
{
    QuickSortHelp(elem, 0, n - 1);
}
```

能用递归实现的算法，一般都能写出非递归，即迭代实现，这里利用一个**辅助栈**，代码如下：
```cpp
typedef struct Region
{
    int low;
    int high;
} Region;

void NonRecursionQuickSort(vector<int> &elem, int n)
{
    stack<Region> regions; // 定义一个栈变量
    Region region;
    region.low = 0;
    region.high = n - 1;
    regions.push(region);
    while (!regions.empty())
    {
        region = regions.top();
        regions.pop();
        int pivot = Partition(elem, region.low, region.high);
        if (pivot - 1 > region.low)
        {
            Region regionLow;
            regionLow.low = region.low;
            regionLow.high = pivot - 1;
            regions.push(regionLow);
        }
        if (pivot + 1 < region.high)
        {
            Region regionHigh;
            regionHigh.low = pivot + 1;
            regionHigh.high = region.high;
            regions.push(regionHigh);
        }
    }
}
```

#### 复杂度分析
快速排序的趟数取决于递归树的深度。如果每次划分即对一个枢轴元素定位后，该元素的左侧和右侧子序列的长度相同，则下一步是将两个长度减半的子序列分别排序，这是最便于估算时间复杂度的理想情况。在 $n$ 个元素中，对一个元素定位所需时间为 $O(n)$，即一次划分 `Partition` 所用时间。若设 $T(n)$ 为对 $n$ 个元素的序列进行排序所需的时间，而且每次对一个元素正确定位后，正好以此枢轴元素为界将此序列划分为长度相等的两个子序列，此时，总的计算时间为：
<center>$$T(n) \leq cn + 2T(\frac{n}{2}) = ... = cn\log{n}+nT(1) = O(n\log{n})$$</center>

其中 $c$ 为常数，$cn$ 表示对枢轴元素定位所需时间 $(O(n))$。

可以证明，快排的平均时间复杂度也是 $O(n\log{n})$。此外，实验结果也表明，快速排序在我们所讨论的所有内部排序算法中是平均性能最好的一个。由于快速排序是递归的，需要有一个栈存放每层递归调用时的指针和参数，最大递归调用层数与递归树的深度一致，理想情况下为 $\lceil\log{(n+1)}\rceil$，即存储空间开销为 $O(\log{n})$。

然而，在数据原本有序的情况下，由于我们每次选择第一个元素作为枢轴元素，这样得到的递归树将成为无分叉的单支树，每次划分只会得到一个比上次少一个元素的子序列，这样就必须经过 $n-1$ 趟才能把所有元素定位，而且第1趟需要经过 $n-1$ 次比较才能找到第1个元素的安放位置，第2趟需要经过 $n-2$ 次比较才能找到第2个元素的安放位置，...，总的比较次数将达到：
<center>$$\sum_{i=1}^{n-1} (n-i)=\frac{n(n-1)}{2}\approx\frac{n^2}{2} = O(n^2)$$</center>

即排序速度退化成冒泡排序，比直接插入排序都慢，且占用附加存储空间(栈)将达到 $O(n)$。

**快速排序是一种不稳定的排序算法。**

## 选择排序(Selection Sort)
---
### 简单选择排序(Simple Selection Sort)
---
#### 基本思想
每一趟在 $n-i (i=1,2,...,n-1)$ 个数据元素($elem[i],elem[i+1],...,elem[n-1]$)中选择最小数据元素作为有序序列的第 $i$ 个数据元素。

简单选择排序的第 $i$ 趟是从 $elem[i],...,elem[n-1]$ 中选择整个序列中第 $i$ 小的元素，并将此元素放在 $elem[i]$ 处，也就是说简单选择排序是从未排序的序列中选择最小元素，接着是次小的，...，第 $i$ 小的，...，依此类推，为寻找下一个最小元素，需检索数据整个的未排序部分，即从 $elem[i]$ 开始一直到 $elem[n-1]$ 结束的这部分序列，但每趟排序都只用交换一次元素位置。即把此趟排序找出的最小元素放置到正确位置(当前排序序列的第一个位置，即 $elem[i]$)，直至倒数第二小的被放到数组的倒数第二个位置，则整个序列排序完成。

#### 代码实现
```cpp
void SimpleSelectionSort(vector<int> &elem, int n)
{
    for (int i = 0; i < n - 1; i++) // 共需n-1趟排序
    {
        int minIndex = i;               // 第i趟排序先访问第i个元素
        for (int j = i + 1; j < n; j++) // 上面已经访问第i个元素，这里从i+1开始遍历此后的子序列
            if (elem[j] < elem[minIndex])
                minIndex = j;
        swap(elem[i], elem[minIndex]); // 将第i趟排序的最小元素放置到第i个位置elem[i]处
    }
}
```

#### 复杂度分析
简单选择排序的外层 $for$ 循环共循环 $n-1$ 次，内层 $for$ 循环共循环 $n-1-i$ 次，可知元素总比较次数为：
<center>$$\sum_{i=0}^{n-2}(n-1-i) = \frac{n(n-1)}{2} = O(n^2)$$</center>

<!-- <blockquote class="blockquote-center">
居中的引用文本，会出现左右两个大引号
</blockquote> -->

而元素移动只在每趟外层 $for$ 末尾发生一次，故仅需 $n-1$ 次，即 $O(n)$，故简单选择排序的时间复杂度为：
<center>$$O(n^2) + O(n) = O(n^2)$$</center>

**简单选择排序是一种不稳定的排序算法。**

### 堆排序(Heap Sort)
---
#### 基本思想
堆排序也是一种给予选择排序的先进排序方法，只需要一个元素的辅助存储空间，关于堆这种数据结构的详细介绍请参见[**此篇**](http://dongdongdong.me/2018/05/03/Algorithm/DataStructure/stack-queue-priority_queue-heap/)。在堆排序中我们会用到最大堆，这里你只需知道它实际是个二叉树，每个非叶结点的关键码都比其子结点的关键码要大，根结点的关键码最大，每次插入或删除结点都可能会破坏符合上述定义的堆结构，因此需要重新调整使其再度成为堆。

堆排序的思想其实不难想到：先将一个无序序列构造成一个最大堆，再将堆顶元素(即最大元素)与末尾元素交换位置，即最大元素被放到序列末尾位置，那么该元素就已经放置到其正确位置，此后调整除去末尾元素的子序列使其重新成为最大堆。重复上述过程，每次调整成堆之后都会将一个堆顶元素，即当前排序序列中最大的元素放置到当前序列中的最后一个位置，即其正确位置上，这样需要经过n-1次调整最终会将所有元素放置到正确位置，排序完成。

当一个含有 $n$ 个结点的二叉树在数组中存储时，你需要知道相关结点的下标计算方法如下：
- 二叉树第一个非叶结点下标：$\frac{n-2}{2}$
- 下标为$i$的结点的父结点下标：$\frac{n-1}{2}$
- 下标为$i$的结点的孩子结点下标：左孩子为 $2i+1$，右孩子为 $2i+2$

#### 代码实现
```cpp
void SiftAdjust(vector<int> &elem, int low, int high) 
{
    // 自上而下调整low元素位置使其符合最大堆定义，此过程又称“筛选”，在堆方法中一般命名为SiftDown
    for (int i = low, j = 2 * low + 1; j <= high; j = 2 * j + 1)
    {
        // i为被调整结点，j为其最大孩子，初始是左孩子2i+1，右孩子是2i+2
        if (j < high && elem[j] < elem[j + 1]) // j指向i的最大孩子
            j++;
        if (elem[i] > elem[j]) // 已经成为最大堆，即父结点关键码大于最大子结点关键码
            break;
        swap(elem[i], elem[j]); // 当前结点下沉，其父结点上浮
        i = j;                  // 当前要调整的结点下降
    }
}

void HeapSort(vector<int> &elem, int n)
{
    int i;
    // 将待排序元素数组构造成堆，从第一个非叶结点开始调整，直到根结点
    for (i = (n - 2) / 2; i >= 0; i--) // 第一个非叶结点下标为(n-2)/2
        SiftAdjust(elem, i, n - 1);

    for (i = n - 1; i > 0; i--)
    {
        // 第i趟堆排序
        // 将当前堆顶元素(最大元素)放到当前序列末尾，即第i个位置即其正确位置
        swap(elem[0], elem[i]);     // 交换当前堆顶与末尾元素
        SiftAdjust(elem, 0, i - 1); // 调整从elem[0...i-1]的子序列成为最大堆
    }
}
```

#### 复杂度分析
若设堆中有 $n$ 个结点，且树的深度为 $k$，则有 $2^{k-1} \leq n < 2^k$，对应的完全二叉树有 $k$ 层。在第 $i$ 层上的结点数 $\leq$ $2^{i-1}$。在第一个for循环中构造初始堆过程中，对每一个非叶结点都调用了一次堆调整算法 `SiftAdjust()`，因此该循环计算时间为：
<center>$$2 \cdot \sum_{i=1}^{k-1} 2^{i-1} \cdot (k-i)$$</center>

其中，$i$ 是层次编号，$2^{i-1}$ 是第i层的最大结点数，$(k-i)$ 是第i层结点所能移动的最大距离(从上往下调整的纵向距离)。

设 $j=k-i$，则有：
<center>$$2 \cdot \sum_{i=1}^{k-1} 2^{i-1} \cdot (k-i)$$</center>

<center>$$=2 \cdot \sum_{j=1}^{k-1} 2^{k-j-1} \cdot j$$</center>

<center>$$=2 \cdot 2^{k-1} \sum_{j=1}^{k-1}\frac{j}{2^j}$$</center>

<center>$$< 2 \cdot n\sum_{j=1}^{k-1}\frac{j}{2^j}$$</center>

<center>$$< 4n = O(n)$$</center>

在第二个 $for$ 循环中，调用了 $n-1$ 次 `SiftAdjust()` 方法，由[**此篇**](http://dongdongdong.me/2018/05/03/Algorithm/DataStructure/stack-queue-priority_queue-heap/)可知该循环的总计算时间为 $O(n\log{n})$，因此堆排序的时间复杂度为 $O(n\log{n})$。该算法的附件存储主要是在第二个 $for$ 循环中用来执行元素交换时所用的一个临时元素，即空间复杂度为 $O(1)$。

**堆排序是一种不稳定的排序算法。**

## 归并排序(Merge Sort)
---
### 基本思想
归并是指将两个有序的子序列合并和一个新的有序子序列，设在初始序列中有 $n$ 个元素，归并排序的基本思想是：将序列看成 $n$ 个有序的子序列，每个序列长度为1，然后两两归并，得到 $\lceil\frac{n}{2}\rceil$ 个长度为2或1的有序子序列，然后再两两归并，...，这样重复下去，直至得到一个长度为 $n$ 的有序子序列，这种排序方法称为 **2-路归并排序**。如果每次将3个有序子序列合并为一个新的有序子序列，则称为 **3-路归并排序**，并可依次类推。对于**内部排序**而言，2-路归并排序就能完全满足实现需要，只有**外部排序**才需要多路归并以减少磁盘IO次数，本节仅讨论2-路归并排序。

### 代码实现
```cpp
void RecursionSimplenMerge(vector<int> &elem, int low, int mid, int high)
{
    // 将有序子序列elem[low...mid]和elem[mid+1...high]归并为新的有序子序列elem[low...high]
    int *tmpElem = new int[high + 1]; // 临时数组用于存储结果数组

    int i, j, k; // 分别表示数组elem[low...mid]、elem[mid+1...high]和结果数组tmpElem中当前元素的下标
    for (i = low, j = mid + 1, k = low; i <= mid && j <= high; k++)
    {
        if (elem[i] <= elem[j])
        {
            // elem[i]较小，先放入结果数组tmpElem
            tmpElem[k] = elem[i];
            i++;
        }
        else
        {
            // elem[j]较小，先放入结果数组tmpElem
            tmpElem[k] = elem[j];
            j++;
        }
    }

    for (; i <= mid; i++, k++)
        tmpElem[k] = elem[i];

    for (; j <= high; j++, k++)
        tmpElem[k] = elem[j];

    for (i = low; i <= high; i++)
        // 将结果tmpElem[low...high]复制到原数组elem[low...high]
        elem[i] = tmpElem[i];

    delete[] tmpElem; // 一定记得new出来的空间要手动delete释放
}

void RecursionSimpleMergeSortHelp(vector<int> &elem, int low, int high)
{
    // 划分区间，合并区间，即完成归并排序
    if (low < high) // low = high 即区间只有一个元素时结束循环，无需再划分
    {
        int mid = (low + high) / 2;               // 划分区间 low~mid 和 mid+1~high
        RecursionSimpleMergeSortHelp(elem, low, mid);      // 对elem[low...mid]进行归并排序
        RecursionSimpleMergeSortHelp(elem, mid + 1, high); // 对elem[mid+1...high]进行归并排序
        RecursionSimpleMerge(elem, low, mid, high);        // 堆elem[low...mid]和elem[mid+1...high]两个区间排序结果进行归并
    }
}

void RecursionSimpleMergeSort(vector<int> &elem, int n)
{
    SimpleMergeSortHelp(elem, 0, n - 1);
}
```

对于上面的归并排序实现，在每次归并时都要为临时 $vector$ 分配存储空间，归并结束后还需释放空间，要花费不少时间，为进一步提高运行速度，可在主归并函数 `RecursionMergeSort()` 中统一为临时 $vector$ 分配存储空间，为作区分，以上代码实现的归并排序算法称之为**简单归并排序**，方法名为 `RecursionSimpleMergeSort()` ，下面经改进过的归并排序方法名为 `RecursionMergeSort()`。
```cpp
void RecursionMerge(vector<int> &elem, vector<int> &tmpElem, int low, int mid, int high)
{
    // 将有序子序列elem[low...mid]和elem[mid+1...high]归并为新的有序子序列elem[low...high]
    int i, j, k; // 分别表示数组elem[low...mid]、elem[mid+1...high]和结果数组tmpElem中当前元素的下标
    for (i = low, j = mid + 1, k = low; i <= mid && j <= high; k++)
    {
        if (elem[i] <= elem[j])
        {
            // elem[i]较小，先放入结果数组tmpElem
            tmpElem[k] = elem[i];
            i++;
        }
        else
        {
            // elem[j]较小，先放入结果数组tmpElem
            tmpElem[k] = elem[j];
            j++;
        }
    }

    for (; i <= mid; i++, k++)
        tmpElem[k] = elem[i];

    for (; j <= high; j++, k++)
        tmpElem[k] = elem[j];

    for (i = low; i <= high; i++)
        // 将结果tmpElem[low...high]复制到原数组elem[low...high]
        elem[i] = tmpElem[i];
}

void RecursionMergeSortHelp(vector<int> &elem, vector<int> &tmpElem, int low, int high)
{
    // 划分区间，合并区间，即完成归并排序
    if (low < high) // low = high 即区间只有一个元素时结束循环，无需再划分
    {
        int mid = (low + high) / 2;                  // 划分区间 low~mid 和 mid+1~high
        MergeSortHelp(elem, tmpElem, low, mid);      // 对elem[low...mid]进行归并排序
        MergeSortHelp(elem, tmpElem, mid + 1, high); // 对elem[mid+1...high]进行归并排序
        Merge(elem, tmpElem, low, mid, high);        // 堆elem[low...mid]和elem[mid+1...high]两个区间排序结果进行归并
    }
}

void RecursionMergeSort(vector<int> &elem, int n)
{
    vector<int> tmpElem(n, 0);
    MergeSortHelp(elem, tmpElem, 0, n - 1);
}
```

另外给出一种形式更简洁的递归版归并排序实现：
```cpp
void RecursionMergeSort(vector<int> &elem, int start, int end)
{
    // 当子序列就只有一个元素的时候就弹出
    if (start == end)
        return;

    // 分治
    int mid = (start + end) / 2;
    MergeSort(elem, start, mid);
    MergeSort(elem, mid + 1, end);

    // 合并
    Merge(elem, start, mid, end);
}

void Merge(vector<int> &elem, int start, int mid, int end)
{
    vector<int> tmpElem(end - start + 1, 0);

    int i = start, j = mid + 1;
    for (int k = start; k <= end; k++)
    {
        if (i > mid)
            // 第一个序列元素已合并完
            tmpElem[k] = elem[j++];
        else if (j > end)
            // 第二个序列元素已合并完
            tmpElem[k] = elem[i++];
        else if (elem[i] > elem[j])
            // 先放较小的元素
            tmpElem[k] = elem[j++];
        else
            // 先放较小的元素
            tmpElem[k] = elem[i++];
    }
    for (int i = start; i <= end; i++)
        // 元素放回原数组
        elem[i] = tmpElem[i];
}
```

与快排一样，能用递归实现的算法，一般都能写出非递归，即迭代实现，思想和递归正好相反，原来的递归过程是将待排序集合一分为二，此后不断划分直至排序集合就剩下一个元素位置，然后不断的合并两个排好序的数组。

所以非递归版本的归并排序的思想为：将数组中的相邻元素两两配对。用 `Merge()` 函数将它们排序，构成 $\frac{n}{2}$ 组长度为2的排序好的子数组段，然后再将它们排序成长度为4的子数组段，如此继续下去，直至整个数组排好序，代码如下：
```cpp
void Merge(vector<int> &elem, int start, int mid, int end)
{
    vector<int> tmpElem(end - start + 1, 0);

    int i = start, j = mid + 1;
    for (int k = start; k <= end; k++)
    {
        if (i > mid)
            // 第一个序列元素已合并完
            tmpElem[k] = elem[j++];
        else if (j > end)
            // 第二个序列元素已合并完
            tmpElem[k] = elem[i++];
        else if (elem[i] > elem[j])
            // 先放较小的元素
            tmpElem[k] = elem[j++];
        else
            // 先放较小的元素
            tmpElem[k] = elem[i++];
    }
    for (int i = start; i <= end; i++)
        // 元素放回原数组
        elem[i] = tmpElem[i];
}

void NonRecursionMergeSort(vector<int> &elem, int n)
{
    int s = 2, i;

    while (s <= n)
    {
        i = 0;
        while (i + s <= n)
        {
            Merge(elem, i, i + s / 2 - 1, i + s - 1);
            i += s;
        }
        //处理末尾残余部分
        Merge(elem, i, i + s / 2 - 1, n - 1);
        s *= 2;
    }
    //最后再从头到尾处理一遍
    Merge(elem, 0, s / 2 - 1, n - 1);
}
```

### 复杂度分析
由于将有序子序列 $elem[low...mid]$ 和 $elem[mid+1...high]$ 归并为有序子序列 $elem[low...high]$ 需要时间 $O(high-low+1)$，可知进行一趟归并需要时间 $O(n)$，设：
<center>$$2^{h-1} < n \leq 2^h$$</center>

在进行第1趟归并后，将得到 $n_1=\lceil\frac{n}{2}\rceil$ 个长度为2或更短的有序子序列，其中 $n_1$ 满足：
<center>$$2^{h-2} \leq n_1 \leq 2^{h-1}$$</center>

在进行第2趟归并后，将得到 $n_2=\lceil\frac{n_1}{2}\rceil$ 个长度为4或更短的有序子序列，其中 $n_2$ 满足：
<center>$$2^{h-3} \leq n_2 \leq 2^{h-2}$$</center>

这样继续下去，在进行第 $h$ 趟归并后，将得到 $n_h=\lceil\frac{n_{h-1}}{2}\rceil$ 个长度为 $2^h$ 或更短的有序子序列，其中 $n_h$ 满足：
<center>$$2^{h-(h+1)} \leq n_h \leq 2^{h-h}$$</center>

由上式可知，$\frac{1}{2} \leq n_h \leq 1$，左移 $n_h=1$，也就是归并躺数为 $h$，再回到开头的式子：
<center>$$2^{h-1} < n \leq 2^h$$</center>

上式各项取以2为底的对数可得：
<center>$$h-1 < \log{n} \leq h$$</center>

从而 $h=\lceil log{n}\rceil$，这样时间复杂度就为 $O(n\lceil\log{n}\rceil)=O(n\log{n})$。

并且，归并排序的时间代价并不依赖于待排序数组的初始排列情况，也就是说归并排序的最好、平均、最差情况的时间复杂度都是 $O(n\log{n})$，这一点比快速排序好，当然在平均情况下，还是快速排序最快(常数因子更小)。

**归并排序是一种稳定的排序算法。**

## 性能对比
---
|排序算法|最好时间|平均时间|最坏时间|辅助空间|稳定性|
|:-----:|:-----:|:----:|:-----:|:----:|:---:|
|直接插入排序|$O(n)$|$O(n^2)$|$O(n^2)$|$O(1)$|稳定|
|折半插入排序|$O(n\log{n})$|$O(n^2)$|$O(n^2)$|$O(1)$|稳定|
|希尔排序|由增量序列决定|由增量序列决定|由增量序列决定|$O(1)$|不稳定|
|冒泡排序|$O(n)$|$O(n^2)$|$O(n^2)$|$O(1)$|稳定|
|快速排序|$O(n\log{n})$|$O(n\log{n})$|$O(n^2)$|$O(\log{n})/O(n)$|不稳定|
|选择排序|$O(n^2)$|$O(n^2)$|$O(n^2)$|$O(1)$|不稳定|
|堆排序|$O(n\log{n})$|$O(n\log{n})$|$O(n\log{n})$|$O(1)$|不稳定|
|归并排序|$O(n\log{n})$|$O(n\log{n})$|$O(n\log{n})$|$O(n)$|稳定|

## 参考资料
---
[1][数据结构与算法(C++版) - 唐宁九主编](https://www.amazon.cn/dp/B001TREOXM)
[2][数据结构(用面向对象方法与C++语言描述) - 殷人昆主编](https://www.amazon.cn/dp/B0011F7UHO)
[3][十大经典排序算法(带动图演示)](http://www.cnblogs.com/onepixel/p/7674659.html)
[4][快速排序详解 - 递归与非递归实现](https://blog.csdn.net/yunzhongguwu005/article/details/9455991)
[5][归并排序详解 - 递归与非递归实现](https://blog.csdn.net/lpjishu/article/details/51290930)
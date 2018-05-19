#include <iostream>
#include <vector>
#include <stack>
using namespace std;

/* 
    直接插入排序 - 稳定
    时间复杂度：最好O(n)，平均O(n^2)，最差O(n^2)
    空间复杂度：O(1)
 */
void StraightInsertSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // 进行第i次插入，待插入元素为elem[i]
        // 前面0~i-1都已有序
        int e = elem[i]; // 暂存待插入元素
        int j;
        for (j = i - 1; j >= 0 && e < elem[j]; j--)
        {
            // 将比e大的都后移一位
            elem[j + 1] = elem[j];
        }
        // 结束上述循环即找到令elem[i] >= elem[j]的位置j
        // 此时elem[i]应插入在elem[j]之后一个位置
        elem[j + 1] = e;
    }
}

/* 
    二分插入排序 - 稳定
    时间复杂度：最好(nlogn)，平均O(n^2)，最差O(n^2)
    空间复杂度：O(1)
 */
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

/* 
    希尔排序 - 不稳定
    时间复杂度：由增量序列incrs决定
    空间复杂度：O(1)
 */
void ShellInsert(vector<int> &elem, int n, int incr)
{
    // 对数组elem作一趟增量为incr的Shell排序
    // 对插入排序作出的修改是子序列中前后相邻记录的增量为incr，而不是1
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
void ShellSort(vector<int> &elem, int n, vector<int> &incr, int t)
{
    for (int k = 0; k < t; k++)
    {
        // 第k趟希尔排序
        ShellInsert(elem, n, incr[k]);
    }
}

/* 
    冒泡排序 - 稳定
    时间复杂度：最好O(n)，平均O(n^2)，最差O(n^2)
    空间复杂度：O(1)
 */
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
                exchange = true;
            }
        }
        if (!exchange) // 本趟排序未发生交换，则排序完成，算法结束
            return;
    }
}

/* 
    快速排序 - 不稳定
    时间复杂度：最好O(nlogn)，平均O(nlogn)，最差O(n^2)
    空间复杂度：最好O(logn)，最差O(n)
 */
int Partition(vector<int> &elem, int low, int high)
{
    while (low < high)
    {
        while (low < high && elem[low] <= elem[high])
            high--;
        swap(elem[low], elem[high]);
        while (low < high && elem[high] >= elem[low])
            low++;
        swap(elem[low], elem[high]);
    }
    return low;
}

// 严蔚敏版本Partition
int Partition2(vector<int> &elem, int low, int high)
{
    int pivotKey = elem[low];
    while (low < high)
    {
        while (low < high && elem[high] >= pivotKey)
            high--;
        elem[low] = elem[high];
        while (low < high && elem[low] <= pivotKey)
            low++;
        elem[high] = elem[low];
    }
    elem[low] = pivotKey;
    return low;
}

void QuickSortHelp(vector<int> &elem, int low, int high)
{
    if (low < high)
    {
        int pivot = Partition(elem, low, high);
        QuickSortHelp(elem, low, pivot - 1);
        QuickSortHelp(elem, pivot + 1, high);
    }
}

// 递归快排
void RecursionQuickSort(vector<int> &elem, int n)
{
    QuickSortHelp(elem, 0, n - 1);
}

typedef struct Region
{
    int low;
    int high;
} Region;

// 非递归快排
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

/* 
    选择排序 - 不稳定
    时间复杂度：最好O(n^2)，平均O(n^2)，最差O(n^2)
    空间复杂度：O(1)
 */
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

/* 
    堆排序 - 不稳定
    时间复杂度：最好O(nlogn)，平均O(nlogn)，最差O(nlogn)
    空间复杂度：O(1)
 */
void SiftAdjust(vector<int> &elem, int low, int high)
{
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

/* 
    归并排序 - 稳定
    时间复杂度：最好O(nlogn)，平均O(nlogn)，最差O(nlogn)
    空间复杂度：O(n)
 */
void SimpleMerge(vector<int> &elem, int low, int mid, int high)
{
    // 将有序子序列elem[low...mid]和elem[mid+1...high]归并为新的有序子序列elem[low...high]
    // int *tmpElem = new int[high + 1]; // 临时数组用于存储结果数组

    vector<int> tmpElem(high + 1, 0); // 用vector替代普通数组

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

    // delete[] tmpElem; // 一定记得new出来的空间要手动delete释放
}

void SimpleMergeSortHelp(vector<int> &elem, int low, int high)
{
    // 划分区间，合并区间，即完成归并排序
    if (low < high) // low = high 即区间只有一个元素时结束循环，无需再划分
    {
        int mid = (low + high) / 2;               // 划分区间 low~mid 和 mid+1~high
        SimpleMergeSortHelp(elem, low, mid);      // 对elem[low...mid]进行归并排序
        SimpleMergeSortHelp(elem, mid + 1, high); // 对elem[mid+1...high]进行归并排序
        SimpleMerge(elem, low, mid, high);        // 堆elem[low...mid]和elem[mid+1...high]两个区间排序结果进行归并
    }
}

// 简单归并排序
void SimpleMergeSort(vector<int> &elem, int n)
{
    SimpleMergeSortHelp(elem, 0, n - 1);
}

void Merge(vector<int> &elem, vector<int> &tmpElem, int low, int mid, int high)
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

void MergeSortHelp(vector<int> &elem, vector<int> &tmpElem, int low, int high)
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

// 改进的归并排序，递归版
void MergeSort(vector<int> &elem, int n)
{
    vector<int> tmpElem(n, 0);
    MergeSortHelp(elem, tmpElem, 0, n - 1);
}

// 精简版递归实现的归并排序
void RecursionMerge(vector<int> &elem, int start, int mid, int end)
{
    vector<int> tmpElem(elem.size(), 0);

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

void RecursionMergeSortHelp(vector<int> &elem, int start, int end)
{
    //当子序列就只有一个元素的时候就弹出
    if (start == end)
        return;

    //分治
    int mid = (start + end) / 2;
    RecursionMergeSortHelp(elem, start, mid);
    RecursionMergeSortHelp(elem, mid + 1, end);

    //合并
    RecursionMerge(elem, start, mid, end);
}

void RecursionMergeSort(vector<int> &elem, int n)
{
    RecursionMergeSortHelp(elem, 0, n - 1);
}

// // 非递归实现的归并排序 - 参考他人
// void NonRecursionMergeSort(vector<int> &elem, int n)
// {
//     int s = 2, i;

//     while (s <= n)
//     {
//         i = 0;
//         while (i + s <= n)
//         {
//             merge(elem, i, i + s / 2 - 1, i + s - 1);
//             i += s;
//         }
//         //处理末尾残余部分
//         merge(elem, i, i + s / 2 - 1, n - 1);
//         s *= 2;
//     }
//     //最后再从头到尾处理一遍
//     merge(elem, 0, s / 2 - 1, n - 1);
// }

// 非递归实现的归并排序
void NonRecursionMerge(vector<int> &elem, int left, int mid, int right)
{
    vector<int> tmpElem(elem.size(), 0);
    int l = left, r = mid + 1, k = left;
    for (; k <= right; k++) // 合并两个区间
    {
        if (l > mid)
            tmpElem[k] = elem[r++];
        else if (r > right)
            tmpElem[k] = elem[l++];
        else if (elem[l] < elem[r])
            tmpElem[k] = elem[l++];
        else
            tmpElem[k] = elem[r++];
    }
    for (int j = left; j <= right; j++)
        elem[j] = tmpElem[j];
}

void NonRecursionMergeSort(vector<int> &elem, int n)
{
    int step = 2;
    for (; step <= n; step *= 2) // 每次对相邻的step个元素进行归并
    {
        bool flag = false;
        int i = 0;
        for (; i < n; i += step) // i指示每个子序列的起始位置，当前子序列含有step个元素(最后一个可能少于step个)
        {
            // 对当前子序列内元素进行归并排序，分成两个区间：[left,mid]和[mid+1,right]
            // 特殊情形 right > n-1，即最后一个区间少于step个元素
            int left = i, mid = i + step / 2 - 1, right = i + step - 1;
            if (right > n - 1) // 最后一个区间少于step个元素，不应再做简单的归并，跳出然后特殊处理
            {
                flag = true;
                break;
            }
            NonRecursionMerge(elem, left, mid, right);
        }
        if (flag) // 特殊处理right>n-1的情况
            NonRecursionMerge(elem, i, i + step / 2 - 1, n - 1);
    }
    NonRecursionMerge(elem, 0, step / 2 - 1, n - 1); // 从头到尾再归并一次
}

int main()
{
    // 测试用例
    vector<int> elem = {18, 8, 56, 9, 68, 8, 11};

    // StraightInsertSort(elem, elem.size());

    // BinaryInsertSort(elem, elem.size());

    // vector<int> incr = {4, 2, 1};
    // ShellSort(elem, elem.size(), incr, incr.size());

    // BubbleSort(elem, elem.size());
    // AdvancedBubbleSort(elem, elem.size());

    // RecursionQuickSort(elem, elem.size());
    // NonRecursionQuickSort(elem, elem.size());

    // SimpleSelectionSort(elem, elem.size());

    // HeapSort(elem, elem.size());

    // SimpleMergeSort(elem, elem.size());
    // MergeSort(elem, elem.size());

    RecursionMergeSort(elem, elem.size());
    // NonRecursionMergeSort(elem, elem.size());

    for (auto e : elem)
        cout << e << " ";

    return 0;
}
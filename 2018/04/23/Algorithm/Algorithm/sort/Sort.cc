#include <iostream>
#include <vector>
#include <stack>
using namespace std;

/* 
    ֱ�Ӳ������� - �ȶ�
    ʱ�临�Ӷȣ����O(n)��ƽ��O(n^2)�����O(n^2)
    �ռ临�Ӷȣ�O(1)
 */
void StraightInsertSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // ���е�i�β��룬������Ԫ��Ϊelem[i]
        // ǰ��0~i-1��������
        int e = elem[i]; // �ݴ������Ԫ��
        int j;
        for (j = i - 1; j >= 0 && e < elem[j]; j--)
        {
            // ����e��Ķ�����һλ
            elem[j + 1] = elem[j];
        }
        // ��������ѭ�����ҵ���elem[i] >= elem[j]��λ��j
        // ��ʱelem[i]Ӧ������elem[j]֮��һ��λ��
        elem[j + 1] = e;
    }
}

/* 
    ���ֲ������� - �ȶ�
    ʱ�临�Ӷȣ����(nlogn)��ƽ��O(n^2)�����O(n^2)
    �ռ临�Ӷȣ�O(1)
 */
void BinaryInsertSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // ���е�i�β��룬������Ԫ��Ϊelem[i]
        // ǰ��0~i-1��������
        int e = elem[i];           // �ݴ������Ԫ��
        int low = 0, high = i - 1; // ��Χ�˵�
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (e > elem[mid])
                low = mid + 1;
            else // e <= elem[mid]
                high = mid - 1;
        }
        // ��������ѭ�����ҵ�elem[i]Ӧ�����λ��Ϊlow
        for (int j = i - 1; j >= low; j--)
            elem[j + 1] = elem[j];

        elem[low] = e;
    }
}

/* 
    ϣ������ - ���ȶ�
    ʱ�临�Ӷȣ�����������incrs����
    �ռ临�Ӷȣ�O(1)
 */
void ShellInsert(vector<int> &elem, int n, int incr)
{
    // ������elem��һ������Ϊincr��Shell����
    // �Բ��������������޸�����������ǰ�����ڼ�¼������Ϊincr��������1
    for (int i = incr; i < n; i++)
    {
        // ��i�˲�������
        int e = elem[i]; // �ݴ������Ԫ��
        int j;
        for (j = i - incr; j >= 0 && e < elem[j]; j -= incr)
        {
            // ���������б�e��ļ�¼�����ƣ�ע�������������к��ƣ���һ��λ����j+incr
            elem[j + incr] = elem[j];
        }
        // ��������ѭ�����ҵ���elem[i] >= elem[j]��λ��j
        // ��ʱelem[i]Ӧ�÷���elem[j]��ͬһ���������е���һ��λ�ã���j+incr
        elem[j + incr] = e;
    }
}

// tΪ�������г���
void ShellSort(vector<int> &elem, int n, vector<int> &incr, int t)
{
    for (int k = 0; k < t; k++)
    {
        // ��k��ϣ������
        ShellInsert(elem, n, incr[k]);
    }
}

/* 
    ð������ - �ȶ�
    ʱ�临�Ӷȣ����O(n)��ƽ��O(n^2)�����O(n^2)
    �ռ临�Ӷȣ�O(1)
 */
void BubbleSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // ��i��ð�����򣬽���i���Ԫ�ط���λ��n-i��
        for (int j = 0; j < n - i; j++)
        {
            // �ڵ�i��ð�������������Ҫִ��n-i�αȽϺͽ�������
            if (elem[j] > elem[j + 1])
                swap(elem[j], elem[j + 1]);
        }
    }
}

void AdvancedBubbleSort(vector<int> &elem, int n)
{
    for (int i = 1; i < n; i++)
    {
        // ��i��ð�����򣬽���i���Ԫ�ط���λ��n-i��
        bool exchange = false;
        for (int j = 0; j < n - i; j++)
        {
            // �ڵ�i��ð�������������Ҫִ��n-i�αȽϺͽ�������
            if (elem[j] > elem[j + 1])
            {
                swap(elem[j], elem[j + 1]);
                exchange = true;
            }
        }
        if (!exchange) // ��������δ������������������ɣ��㷨����
            return;
    }
}

/* 
    �������� - ���ȶ�
    ʱ�临�Ӷȣ����O(nlogn)��ƽ��O(nlogn)�����O(n^2)
    �ռ临�Ӷȣ����O(logn)�����O(n)
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

// ��ε���汾Partition
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

// �ݹ����
void RecursionQuickSort(vector<int> &elem, int n)
{
    QuickSortHelp(elem, 0, n - 1);
}

typedef struct Region
{
    int low;
    int high;
} Region;

// �ǵݹ����
void NonRecursionQuickSort(vector<int> &elem, int n)
{
    stack<Region> regions; // ����һ��ջ����
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
    ѡ������ - ���ȶ�
    ʱ�临�Ӷȣ����O(n^2)��ƽ��O(n^2)�����O(n^2)
    �ռ临�Ӷȣ�O(1)
 */
void SimpleSelectionSort(vector<int> &elem, int n)
{
    for (int i = 0; i < n - 1; i++) // ����n-1������
    {
        int minIndex = i;               // ��i�������ȷ��ʵ�i��Ԫ��
        for (int j = i + 1; j < n; j++) // �����Ѿ����ʵ�i��Ԫ�أ������i+1��ʼ�����˺��������
            if (elem[j] < elem[minIndex])
                minIndex = j;
        swap(elem[i], elem[minIndex]); // ����i���������СԪ�ط��õ���i��λ��elem[i]��
    }
}

/* 
    ������ - ���ȶ�
    ʱ�临�Ӷȣ����O(nlogn)��ƽ��O(nlogn)�����O(nlogn)
    �ռ临�Ӷȣ�O(1)
 */
void SiftAdjust(vector<int> &elem, int low, int high)
{
    for (int i = low, j = 2 * low + 1; j <= high; j = 2 * j + 1)
    {
        // iΪ��������㣬jΪ������ӣ���ʼ������2i+1���Һ�����2i+2
        if (j < high && elem[j] < elem[j + 1]) // jָ��i�������
            j++;
        if (elem[i] > elem[j]) // �Ѿ���Ϊ���ѣ��������ؼ����������ӽ��ؼ���
            break;
        swap(elem[i], elem[j]); // ��ǰ����³����丸����ϸ�
        i = j;                  // ��ǰҪ�����Ľ���½�
    }
}

void HeapSort(vector<int> &elem, int n)
{
    int i;
    // ��������Ԫ�����鹹��ɶѣ��ӵ�һ����Ҷ��㿪ʼ������ֱ�������
    for (i = (n - 2) / 2; i >= 0; i--) // ��һ����Ҷ����±�Ϊ(n-2)/2
        SiftAdjust(elem, i, n - 1);

    for (i = n - 1; i > 0; i--)
    {
        // ��i�˶�����
        // ����ǰ�Ѷ�Ԫ��(���Ԫ��)�ŵ���ǰ����ĩβ������i��λ�ü�����ȷλ��
        swap(elem[0], elem[i]);     // ������ǰ�Ѷ���ĩβԪ��
        SiftAdjust(elem, 0, i - 1); // ������elem[0...i-1]�������г�Ϊ����
    }
}

/* 
    �鲢���� - �ȶ�
    ʱ�临�Ӷȣ����O(nlogn)��ƽ��O(nlogn)�����O(nlogn)
    �ռ临�Ӷȣ�O(n)
 */
void SimpleMerge(vector<int> &elem, int low, int mid, int high)
{
    // ������������elem[low...mid]��elem[mid+1...high]�鲢Ϊ�µ�����������elem[low...high]
    // int *tmpElem = new int[high + 1]; // ��ʱ�������ڴ洢�������

    vector<int> tmpElem(high + 1, 0); // ��vector�����ͨ����

    int i, j, k; // �ֱ��ʾ����elem[low...mid]��elem[mid+1...high]�ͽ������tmpElem�е�ǰԪ�ص��±�
    for (i = low, j = mid + 1, k = low; i <= mid && j <= high; k++)
    {
        if (elem[i] <= elem[j])
        {
            // elem[i]��С���ȷ���������tmpElem
            tmpElem[k] = elem[i];
            i++;
        }
        else
        {
            // elem[j]��С���ȷ���������tmpElem
            tmpElem[k] = elem[j];
            j++;
        }
    }

    for (; i <= mid; i++, k++)
        tmpElem[k] = elem[i];

    for (; j <= high; j++, k++)
        tmpElem[k] = elem[j];

    for (i = low; i <= high; i++)
        // �����tmpElem[low...high]���Ƶ�ԭ����elem[low...high]
        elem[i] = tmpElem[i];

    // delete[] tmpElem; // һ���ǵ�new�����Ŀռ�Ҫ�ֶ�delete�ͷ�
}

void SimpleMergeSortHelp(vector<int> &elem, int low, int high)
{
    // �������䣬�ϲ����䣬����ɹ鲢����
    if (low < high) // low = high ������ֻ��һ��Ԫ��ʱ����ѭ���������ٻ���
    {
        int mid = (low + high) / 2;               // �������� low~mid �� mid+1~high
        SimpleMergeSortHelp(elem, low, mid);      // ��elem[low...mid]���й鲢����
        SimpleMergeSortHelp(elem, mid + 1, high); // ��elem[mid+1...high]���й鲢����
        SimpleMerge(elem, low, mid, high);        // ��elem[low...mid]��elem[mid+1...high]�����������������й鲢
    }
}

// �򵥹鲢����
void SimpleMergeSort(vector<int> &elem, int n)
{
    SimpleMergeSortHelp(elem, 0, n - 1);
}

void Merge(vector<int> &elem, vector<int> &tmpElem, int low, int mid, int high)
{
    // ������������elem[low...mid]��elem[mid+1...high]�鲢Ϊ�µ�����������elem[low...high]
    int i, j, k; // �ֱ��ʾ����elem[low...mid]��elem[mid+1...high]�ͽ������tmpElem�е�ǰԪ�ص��±�
    for (i = low, j = mid + 1, k = low; i <= mid && j <= high; k++)
    {
        if (elem[i] <= elem[j])
        {
            // elem[i]��С���ȷ���������tmpElem
            tmpElem[k] = elem[i];
            i++;
        }
        else
        {
            // elem[j]��С���ȷ���������tmpElem
            tmpElem[k] = elem[j];
            j++;
        }
    }

    for (; i <= mid; i++, k++)
        tmpElem[k] = elem[i];

    for (; j <= high; j++, k++)
        tmpElem[k] = elem[j];

    for (i = low; i <= high; i++)
        // �����tmpElem[low...high]���Ƶ�ԭ����elem[low...high]
        elem[i] = tmpElem[i];
}

void MergeSortHelp(vector<int> &elem, vector<int> &tmpElem, int low, int high)
{
    // �������䣬�ϲ����䣬����ɹ鲢����
    if (low < high) // low = high ������ֻ��һ��Ԫ��ʱ����ѭ���������ٻ���
    {
        int mid = (low + high) / 2;                  // �������� low~mid �� mid+1~high
        MergeSortHelp(elem, tmpElem, low, mid);      // ��elem[low...mid]���й鲢����
        MergeSortHelp(elem, tmpElem, mid + 1, high); // ��elem[mid+1...high]���й鲢����
        Merge(elem, tmpElem, low, mid, high);        // ��elem[low...mid]��elem[mid+1...high]�����������������й鲢
    }
}

// �Ľ��Ĺ鲢���򣬵ݹ��
void MergeSort(vector<int> &elem, int n)
{
    vector<int> tmpElem(n, 0);
    MergeSortHelp(elem, tmpElem, 0, n - 1);
}

// �����ݹ�ʵ�ֵĹ鲢����
void RecursionMerge(vector<int> &elem, int start, int mid, int end)
{
    vector<int> tmpElem(elem.size(), 0);

    int i = start, j = mid + 1;
    for (int k = start; k <= end; k++)
    {
        if (i > mid)
            // ��һ������Ԫ���Ѻϲ���
            tmpElem[k] = elem[j++];
        else if (j > end)
            // �ڶ�������Ԫ���Ѻϲ���
            tmpElem[k] = elem[i++];
        else if (elem[i] > elem[j])
            // �ȷŽ�С��Ԫ��
            tmpElem[k] = elem[j++];
        else
            // �ȷŽ�С��Ԫ��
            tmpElem[k] = elem[i++];
    }
    for (int i = start; i <= end; i++)
        // Ԫ�طŻ�ԭ����
        elem[i] = tmpElem[i];
}

void RecursionMergeSortHelp(vector<int> &elem, int start, int end)
{
    //�������о�ֻ��һ��Ԫ�ص�ʱ��͵���
    if (start == end)
        return;

    //����
    int mid = (start + end) / 2;
    RecursionMergeSortHelp(elem, start, mid);
    RecursionMergeSortHelp(elem, mid + 1, end);

    //�ϲ�
    RecursionMerge(elem, start, mid, end);
}

void RecursionMergeSort(vector<int> &elem, int n)
{
    RecursionMergeSortHelp(elem, 0, n - 1);
}

// // �ǵݹ�ʵ�ֵĹ鲢���� - �ο�����
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
//         //����ĩβ���ಿ��
//         merge(elem, i, i + s / 2 - 1, n - 1);
//         s *= 2;
//     }
//     //����ٴ�ͷ��β����һ��
//     merge(elem, 0, s / 2 - 1, n - 1);
// }

// �ǵݹ�ʵ�ֵĹ鲢����
void NonRecursionMerge(vector<int> &elem, int left, int mid, int right)
{
    vector<int> tmpElem(elem.size(), 0);
    int l = left, r = mid + 1, k = left;
    for (; k <= right; k++) // �ϲ���������
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
    for (; step <= n; step *= 2) // ÿ�ζ����ڵ�step��Ԫ�ؽ��й鲢
    {
        bool flag = false;
        int i = 0;
        for (; i < n; i += step) // iָʾÿ�������е���ʼλ�ã���ǰ�����к���step��Ԫ��(���һ����������step��)
        {
            // �Ե�ǰ��������Ԫ�ؽ��й鲢���򣬷ֳ��������䣺[left,mid]��[mid+1,right]
            // �������� right > n-1�������һ����������step��Ԫ��
            int left = i, mid = i + step / 2 - 1, right = i + step - 1;
            if (right > n - 1) // ���һ����������step��Ԫ�أ���Ӧ�����򵥵Ĺ鲢������Ȼ�����⴦��
            {
                flag = true;
                break;
            }
            NonRecursionMerge(elem, left, mid, right);
        }
        if (flag) // ���⴦��right>n-1�����
            NonRecursionMerge(elem, i, i + step / 2 - 1, n - 1);
    }
    NonRecursionMerge(elem, 0, step / 2 - 1, n - 1); // ��ͷ��β�ٹ鲢һ��
}

int main()
{
    // ��������
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
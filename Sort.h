/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		STL
 * 文件名 :		Sort.h
 * 作者 :			肖国栋
 *				1799553128@qq.com
 * 创建日期 :		2017-11-20
 * 版本 :			XXXX.XX.XX
 * 相关文件 :
 * 文件实现功能 : insert_sort,shell_sort,select_sort,heap_sort,
 *				bubble_sort,quick_sort,merge_sort*
 * 备注 :
 * 修改记录 :
 *
 */

#ifndef __SORT_H__
#define __SORT_H__

#pragma once
#include <iostream>
#include <stack>
using namespace std;

/***********插入排序*************/
void InsertSort(int *a, int n)
{
	//把第i个数插入到[0, i-1]有序区间内，使插入后的序列人保持有序
	for (int i = 1; i < n; ++i){
		int key = a[i];//
		int cur = i - 1;
		while (cur >= 0 && key < a[cur]){
			a[cur + 1] = a[cur];//移动数据
			--cur;
		}
		a[cur+1] = key;
	}
}

/***********希尔排序*************/
void ShellSort(int *a, int n)
{
	//在插入排序的基础上分区间
	int gap = n;
	while (gap > 1){
		gap = gap / 3 + 1;
		for (int i = gap; i < n; ++i){
			int key = a[i];
			int end = i - gap;
			while (end >= 0 && key < a[end]){
				a[end + gap] = a[end];
				end -= gap;
			}
			a[end + gap] = key;
		}
	}
}

/***********选择排序*************/
void SelectSort(int *a, int n)
{
	//在[start, end]区间内选一个最小的、一个最大的值， 分别与start、end交换， 缩小区间重复进行
	int end = n;
	for (int start = 0; start < end; ++start,--end){
		int minIdx = start;
		int maxIdx = start;
		for (int cur = start + 1; cur < end; ++cur){
			if (a[cur] < a[minIdx])
				minIdx = cur;

			if (a[cur] > a[maxIdx])
				maxIdx = cur;
		}
		swap(a[start], a[minIdx]);
		if (maxIdx == start)
			maxIdx = minIdx;
		swap(a[end-1], a[maxIdx]);
		//PrintArray(a, n);
	}
}

/***********堆排序*************/
//向下调整
static void __AdjustDown(int *a, int size, int n)
{
	int parent = n;
	int child = n * 2 + 1;
	while (child < size){
		if (child + 1 < size && a[child + 1] > a[child])
			++child;

		if (a[parent] < a[child]){
			swap(a[parent], a[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
			break;
	}
}
void HeapSort(int *a, int n)
{
	//建堆
	for (int i = n / 2 - 1; i >= 0; --i)
		__AdjustDown(a, n, i);

	//排序
	int i = n;
	while(i>1){
		swap(a[0], a[i-1]);
		--i;
		__AdjustDown(a, i, 0);
	}
}

/***********冒泡排序*************/
void BubbleSort(int *a, int n)
{
	int lastSwap = n;
	int pos = lastSwap;
	while (lastSwap > 1){
		pos = 0;
		for (int i = 1; i < lastSwap; ++i){
			if (a[i - 1] > a[i]){
				pos = i;
				swap(a[i - 1], a[i]);
			}
		}
		lastSwap = pos;
	}
}

/***********快速排序*************/
//左右指针法
static int __PartSort_1(int *a, int left, int right)
{
	int &key = a[right];
	while (left < right){
		while (left < right && a[left] <= key)//从前往后找到一个比key大的
			++left;
		while (left < right && a[right] >= key)//从后往前找到一个比key小的
			--right;
		swap(a[left],a[right]);//交换
	}
	swap(a[left], key);//将key放的正确位置
	return left;
}

//挖坑法
static int __PartSort_2(int *a, int left, int right)
{
	int key = a[right];
	while (left < right){
		while (left < right && a[left] <= key)
			++left;
		swap(a[left], a[right]);

		while (left < right && a[right] >= key)
			--right;
		swap(a[left], a[right]);
	}

	return left;
}

//前后指针法
static int __PartSort_3(int *a, int left, int right)
{
	int &key = a[right];
	int cur = 0;
	int prev = -1;
	while (cur < right){
		if (a[cur] <= key && ++prev != cur)
			swap(a[prev], a[cur]);
		++cur;
	}

	swap(a[++prev], key);
	return prev;
}

//[left, rigth]//左闭右闭的区间
static void __QuickSort(int *a, int left, int right)
{
	if (left >= right)
		return;
	int mid = __PartSort_3(a, left, right);
	__QuickSort(a, left, mid - 1);
	__QuickSort(a, mid+1, right);
}

void QuickSort(int *a, int n)
{
	__QuickSort(a, 0, n-1);
}
//非递归
void QuickSortNoR(int *a, int n)
{
	int left = 0;
	int right = n - 1;
	int mid;
	stack<int> s;
	s.push(right);
	s.push(left);

	while (!s.empty()){
		left = s.top();
		s.pop();
		right = s.top();
		s.pop();

		mid = __PartSort_1(a, left, right);
		
		if (left < mid - 1){
			s.push(mid - 1);
			s.push(left);
		}

		if (mid + 1 < right){
			s.push(right);
			s.push(mid+1);
		}
	}
}

/***********归并排序*************/
//[left, rigth]//左闭右闭的区间
//进行归并
static void __Merge(int *a, int left, int mid, int right, int *tmp)
{
	int i = left;
	int j = mid + 1;
	int k = left;
	while (i <= mid && j <= right){
		if (a[i] <= a[j])
			tmp[k++] = a[i++];
		else
			tmp[k++] = a[j++];
	}

	while (i <= mid)
		tmp[k++] = a[i++];

	while (j <= right)
		tmp[k++] = a[j++];

	memcpy(a + left, tmp + left, (right - left + 1)*sizeof(int));
}

static void __MergeSort(int *a, int left, int right, int *tmp)
{
	if (left >= right)
		return;

	int mid = left + (right - left) / 2;
	__MergeSort(a, left, mid, tmp);//左区间有序
	__MergeSort(a, mid +1, right, tmp);//右区间有序
	__Merge(a, left, mid, right, tmp);//将左右有序区间的归并
}

void MergeSort(int *a, int n) 
{
	int *tmp = new int[n];//将归并后的数据先放到辅助空间，结束后再拷回来
	__MergeSort(a, 0, n-1, tmp);
	delete[] tmp;
}

////////////非比较排序////////////
/***********计数排序*************/
void CountSort(int * a, int n)
{
	int max = a[0];//区间内最大值
	int min = a[0];//区间内最小值

	for (int i = 1; i < n; ++i){
		if (max < a[i])
			max = a[i];

		if (min > a[i])
			min = a[i];
	}

	int range = max - min +1;//区间大小
	int *tmp = new int[range];
	memset(tmp, 0, sizeof(int)*range);

	int i = 0;
	for (; i < n; ++i)
		++tmp[a[i] - min];

	i = 0;
	for (int j = 0; j < range; ++j){
		while (0 != tmp[j]){
			a[i] = j + min;
			--tmp[j];
			++i;
		}
	}
}


//获取数组中数的最高位数{8, 7, 19, 26, 130} -> 3	{90， 2， 100， 1024} -> 4
static int __GetMaxDigit(int *a, int n)
{
	int digit = 0;
	int base = 1;

	for (int i = 0; i < n; ++i){
		while (0 != a[i] / base){	// 8   7		19		26		130	...		
			++digit;				// 1   1		2		2		3	...
			base *= 10;			// 10  10	100		100		100	...
		}
	}

	return digit;
}

/***********基数排序*************/
void RadixSort(int *a, const int n)
{
	int maxDigit = __GetMaxDigit(a, n);
	int *tmp = new int[n];
	int base = 1;//用于确定是第几位

	for (int digit = 0; digit < maxDigit; ++digit){
		int count[10] = { 0 };
		int start[10] = { 0 };

		//统计digit位为i的数据个数
		int i = 0;
		for (; i < n; ++i)
			++count[(a[i]/base)%10];

		//统计数据的起始位置
		i = 1;
		for(; i < 10; ++i)
			start[i] = start[i-1] + count[i-1];

		//按位排序
		i = 0;
		for (; i < n; ++i){
			int& idx = start[a[i] / base % 10];//计算a[i]在数组中的位置
			tmp[idx] = a[i];
			++idx;
		}

		base *= 10;
		memcpy(a, tmp, sizeof(int)*n);//将排好的数据拷贝到原空间
	}

	delete tmp;
}

#endif

#if TESTSORT
//测试

void TestSort()
{

#if 1
	const int N = 10;
	//int a[N] = {2, 0, 400, 9, 3, 6, 8, 7, 19 ,5};
	//int a[N] = {2, 0, 5, 9, 3, 5, 8, 7, 1 ,5};
	int a[N] = {4,2,1,0,3,5,6,7,8,9};
#else
	const int N = 100;
	int a[N];
	RandomArray(a, N);
#endif
	//PrintArray(a, N);
	//InsertSort(a, N);
	//ShellSort(a, N);
	//SelectSort(a, N);
	//HeapSort(a, N);
	BubbleSort(a, N);
	//QuickSort(a, N);
	//QuickSortNoR(a, N);
	//MergeSort(a, N);
	//CountSort(a, N);
	//RadixSort(a,N);
	PrintArray(a, N);
	IsIncresing(a, N);
}

#endif










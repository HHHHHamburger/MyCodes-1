// ��������.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

int RandomInRange(int start, int end)
{
	return rand() % (end - start) + start;
}

void Swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

//ʹ�õ�һ��������Ϊ��׼��
void quick_sort(int *a, int start,int end)
{
	int size = end - start;
	if (a == NULL || size <= 0)
		return;
	int i=start, j=end-1, key=a[start];
	while (i<j)
	{
		while (i < j&&a[j] > key)--j;
		a[i] = a[j];
		while (i < j&&a[i] < key)++i;
		a[j] = a[i];
	}
	a[i] = key;
	quick_sort(a, start, i - 1);
	quick_sort(a, i + 1, end);
}


//ʹ���������Ϊ��׼��
int Partition(int data[], int length, int start, int end)
{
	//�׳��쳣
	if (data == NULL || length <= 0 || start < 0 || end >= length)
		throw exception("Invalid Parameters");
	//����һ�����λ��
	int index = RandomInRange(start,end);

	//����λ�������һ��Ԫ�ؽ���
	Swap(&data[index], &data[end]);

	int small = start - 1;
	//++smallָ����һ�ν���֮�����һ��λ�ã���
	//data[index]С��data[end]�򽻻�
	//small֮ǰ��ȫ������С��data[end]��Ԫ��
	for (index = start; index < end; index++)
	{
		if (data[index] < data[end])
		{
			++small;
			if (small != index)
				Swap(&data[index], &data[small]);
		}
	}
	++small;
	Swap(&data[small], &data[end]);
	//��������Ͻ�����small֮ǰȫ��ΪС��data[small]��Ԫ��
	//small֮��ȫ��Ϊ����data[small]��Ԫ��

	return small;
}

void QuickSort(int data[], int length, int start, int end)
{
	if (start == end)
		return;

	int index = Partition(data, length, start, end);
	if (index > start)
		QuickSort(data, length, start, index - 1);
	if (index < end)
		QuickSort(data, length, index + 1, end);
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(NULL));

	int a[] = { 2,3,1,8,6,7,4 };
	//quick_sort(a, 0, 5);
	QuickSort(a, 7, 0, 6);
	for (int i = 0; i < 7;i++)
		cout << a[i] << " ";
	cin.get();
	return 0;
}


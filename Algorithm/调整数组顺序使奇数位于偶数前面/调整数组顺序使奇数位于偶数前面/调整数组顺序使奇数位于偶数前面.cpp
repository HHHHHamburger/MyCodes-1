// ��������˳��ʹ����λ��ż��ǰ��.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:����һ���������飬ʵ��һ�����������������������ֵ�˳��
//ʹ����������λ�������ǰ�벿�֣�����ż��λ������ĺ�벿��
#include "stdafx.h"
#include <iostream>
using namespace std;

//����������������㷨��û�����õĿ���չ��
void ReorderOddEven(int *numbers, unsigned int length)
{
	if (numbers == NULL || length == 0)
		return;
	int oddIndex=0, evenIndex=length-1;
	while (oddIndex < evenIndex)
	{
		while ((oddIndex<evenIndex)&&((numbers[oddIndex] & 1) == 1))oddIndex++;
		while ((oddIndex<evenIndex) && ((numbers[evenIndex] & 1) == 0))evenIndex--;
		if (oddIndex <= evenIndex)
		{
			int tmp = numbers[oddIndex];
			numbers[oddIndex] = numbers[evenIndex];
			numbers[evenIndex] = tmp;
		}
	}
}

//����������չ�Ե��㷨,�ϸߵ�������
void Reorder(int *numbers, unsigned int length, bool(*func)(int))
{
	if (numbers == NULL || length == 0)
		return;
	int *start = numbers, *end = numbers + length - 1;

	while (start < end)
	{
		while (start < end&&!func(*start))
			start++;
		while (start < end&&func(*end))
			end--;

		if (start < end)
		{
			int tmp = *start;
			*start = *end;
			*end = tmp;
		}
	}
}

bool isEven(int n)
{
	return (n & 1) == 0;
}

void Test(int *original, int *expected, unsigned int length)
{
	cout << "Test For:"<<endl;
	if (original!=NULL)
	for (int i = 0; i < length; i++)
	{
		cout << original[i] << " ";
	}
	cout << endl;
	if (expected!=NULL)
	for (int i = 0; i < length; i++)
	{
		cout << expected[i] << " ";
	}
	cout << endl;
//	ReorderOddEven(original, length);
	Reorder(original, length, isEven);
	cout << "now:" << endl;
	if (original != NULL)
	for (int i = 0; i < length; i++)
	{
		cout << original[i] << " ";
	}
	cout << endl;
	if ((original == NULL&&expected == NULL)|| memcmp(original, expected, sizeof(int)*length) == 0)
		cout << "Test Passed." << endl;
	else
		cout << "Test Failed." << endl;
}

void Test1()
{
	int original[] = { 3, 2, 1, 5, 4 };
	int expected[] = { 3, 5, 1, 2, 4 };
	Test(original, expected, 5);
}

void Test2()
{
	int original[] = { 1, 2, 3, 4, 5 };
	int expected[] = { 1, 5, 3, 4, 2 };
	Test(original, expected, 5);
}

void Test3()
{
	int original[] = { 2, 2, 2, 2, 4 };
	int expected[] = { 2, 2, 2, 2, 4 };
	Test(original, expected, 5);
}

void Test4()
{
	int original[] = { 1, 1, 1, 3, 1 };
	int expected[] = { 1, 1, 1, 3, 1 };
	Test(original, expected, 5);
}

void Test5()
{
	Test(NULL, NULL, 4);
}



int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	cin.get();
	return 0;
}


// ��ת�������С����.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:��һ�������ʼ�����ɸ�Ԫ�ذᵽ�����ĩβ������
//��֮Ϊ�������ת������һ����������������һ����ת��
//�����ת�������СԪ�ء�
//��������{3��4��5��1��2}Ϊ{1��2��3��4��5}��һ����ת��
//���������СֵΪ1

#include "stdafx.h"
#include <iostream>
using namespace std;


//˳�������С��Ԫ��
int MinInOrder(int numbers[], int index1, int index2)
{
	int result = numbers[index1];
	for (int i = index1 + 1; i <= index2; i++)
	{
		if (result > numbers[i])
			result = numbers[i];
	}
	return result;
}


//�ݹ�ⷨ
int Min(int numbers[], int start, int end)
{
	if (numbers == NULL || start < 0 || end < start)
		throw exception("Wrong Array!");

	if (numbers[start] < numbers[end])return numbers[start];

	if (start == end - 1)return numbers[end];

	int mid = (start + end) / 2;

	if (numbers[start] == numbers[end] &&
		numbers[start] == numbers[mid])
		return MinInOrder(numbers, start, end);

	if (numbers[mid] >= numbers[start])
		return Min(numbers, mid, end);
	else if (numbers[mid] <= numbers[end])
		return Min(numbers, start, mid);
}

//ѭ���ⷨ
int Min(int numbers[], int length)
{
	if (numbers == NULL || length <= 0)
		throw exception("Wrong Array!");

	int index1 = 0;
	int index2 = length - 1;
	int indexMid = index1;

	while (numbers[index1] >= numbers[index2])
	{
		if (index2 - index1 == 1)
		{
			indexMid = index2;
			break;
		}

		indexMid = (index1 + index2) / 2;

		if (numbers[index1] == numbers[index2] &&
			numbers[indexMid] == numbers[index1])
			return MinInOrder(numbers, index1, index2);

		if (numbers[indexMid] >= numbers[index1])
			index1 = indexMid;
		else if (numbers[indexMid] <= numbers[index2])
			index2 = indexMid;
	}
	return numbers[indexMid];
}

//�������룬��������������һ����ת
void Test1()
{
	int a[] = { 3, 4, 5, 1, 2 };
	int fact = Min(a, 0, 4);
	int fact1 = Min(a, 5);
	if (fact == 1&&fact1==1)
		cout << "Test 1 Passed!";
	else
		cout << "Test 1 Failed!";
	cout << endl;
}

//ȫ��Ϊ�ظ�����
void Test2()
{
	int a[] = { 3,3,3,3,3};
	int fact = Min(a, 0, 4);
	int fact1 = Min(a, 5);
	if (fact == 3&&fact1==3)
		cout << "Test 2 Passed!";
	else
		cout << "Test 2 Failed!";
	cout << endl;
}
//����NULL
void Test3()
{
	int *a = NULL;
	try
	{
		int fact = Min(a, 0, 4);
		int fact1 = Min(a, 5);
	}
	catch (exception e)
	{
		if (!strcmp(e.what(),"Wrong Array!"))
			cout << "Test 3 Passed!";
		else
			cout << "Test 3 Failed!";
	}
	cout << endl;
}
//���ظ������֣������ظ������ָպ��ǵ�һ�����ֺ����һ������
void Test4()
{
	int a[] = { 1, 0, 1, 1, 1 };
	int fact = Min(a, 0, 4);
	int fact1 = Min(a, 5);
	if (fact == 0&&fact1==0)
		cout << "Test 4 Passed!";
	else
		cout << "Test 4 Failed!";
	cout << endl;
}
//���ظ����֣������ظ������ָպõ���С������
void Test5()
{
	int a[] = { 3, 4, 5, 1, 1, 2 };
	int fact = Min(a, 0, 5);
	int fact1 = Min(a, 6);
	if (fact == 1&&fact1==1)
		cout << "Test 5 Passed!";
	else
		cout << "Test 5 Failed!";
	cout << endl;
}
//���ظ����֣����ظ������ֲ��ǵ�һ�����ֺ����һ������
void Test6()
{
	int a[] = { 3, 4, 5, 1, 2, 2 };
	int fact = Min(a, 0, 5);
	int fact1 = Min(a, 6);
	if (fact == 1&&fact1==1)
		cout << "Test 6 Passed!";
	else
		cout << "Test 6 Failed!";
	cout << endl;
}
//�����������飬��ת0��Ԫ�أ�Ҳ���ǵ����������鱾��
void Test7()
{
	int a[] = { 1, 2, 3, 4, 5 };
	int fact = Min(a, 0, 4);
	int fact1 = Min(a, 5);
	if (fact == 1&&fact1==1)
		cout << "Test 7 Passed!";
	else
		cout << "Test 7 Failed!";
	cout << endl;
}
//������ֻ��һ������
void Test8()
{
	int a[] = { 2 };
	int fact = Min(a, 0, 0);
	int fact1 = Min(a, 1);
	if (fact == 2&&fact1==2)
		cout << "Test 8 Passed!";
	else
		cout << "Test 8 Failed!";
	cout << endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	Test7();
	Test8();
	cin.get();
	return 0;
}


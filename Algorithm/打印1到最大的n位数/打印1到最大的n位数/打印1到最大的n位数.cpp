// ��ӡ1������nλ��.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:��������n,��˳���ӡ����1������nλʮ������������
//����3�����ӡ��1��2��3һֱ��������λ����999
//����Ӧע�������
#include "stdafx.h"
#include <iostream>
using namespace std;

bool AddToNumbers(char *numbers)
{
	bool isOverflow = false;
	int nLength = strlen(numbers);
	int nTakeOver = 0;

	for (int i = nLength - 1; i >= 0; i--)
	{
		int nSum = numbers[i] - '0' + nTakeOver;
		if (i == nLength - 1)
			nSum++;
		if (nSum >= 10)
		{
			if (i == 0)
				isOverflow = true;
			else
			{
				nSum -= 10;
				nTakeOver = 1;
				numbers[i] = '0' + nSum;
			}
		}
		else
		{
			numbers[i] = '0' + nSum;
			break;
		}
	}
	return isOverflow;
}

void PrintNumbers(char *numbers)
{
	int nLength = strlen(numbers);
	bool frontZeroFlag = true;
	for (int i = 0; i < nLength;i++)
	if (frontZeroFlag == true && numbers[i] == '0')
		continue;
	else
	{
		frontZeroFlag = false;
		cout << numbers[i];
	}
	cout << " ";
}

void PrintToMaxDigits_Solution1(int n)
{
	if (n <= 0)
		return;
	char *numbers = new char[n + 1];
	memset(numbers, '0', n);
	numbers[n] = '\0';
	while (!AddToNumbers(numbers))
		PrintNumbers(numbers);
	cout << endl;
	delete[] numbers;
}

void PrintToMaxOfNDigitsRecursively(char *numbers, int length, int index)
{
	if (index == length - 1)
	{
		PrintNumbers(numbers);
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		numbers[index + 1] = i + '0';
		PrintToMaxOfNDigitsRecursively(numbers, length, index + 1);
	}
}

void PrintToMaxDigits_Solution2(int n)
{
	if (n <= 0)
		return;
	char *numbers = new char[n + 1];
	numbers[n] = '\0';
	for (int i = 0; i < 10; i++)
	{
		numbers[0] = i + '0';
		PrintToMaxOfNDigitsRecursively(numbers, n, 0);
	}
	delete[] numbers;
}


void Test(int n)
{
	printf("Test for %d begins:\n", n);

//	PrintToMaxDigits_Solution1(n);
	PrintToMaxDigits_Solution2(n);
	printf("Test for %d ends.\n", n);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Test(1);
	Test(2);
	Test(3);
	Test(0);
	Test(-1);
	cin.get();
	return 0;
}


// ��ֵ�������η�.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:ʵ�ֺ���double Power(double base, int exponent)����
//base��exponent�η�������ʹ�ÿ⺯����ͬʱ����Ҫ���Ǵ�������
#include "stdafx.h"
#include <iostream>
#include <ctime>
using namespace std;

//#define MINE	//�Լ�������
#define BOOK	//���Ͻⷨ
#define NEW_SOLUTION	//��Ч��N�η���
bool g_InvalidInput = false;
bool equal(double num1, double num2);
double PowerWithUnsignedExponent(double base, unsigned int exponent);

//�Լ�������
#ifdef MINE
double Power(double base, int exponent)
{
	double result = 1.0;
	if (exponent == 0)return 1;
	else if (equal(base, 0))
	{
		if(exponent<0)g_InvalidInput = true;
		return 0;
	}
	else if (exponent > 0)
	{
		for (int i = 0; i < exponent; i++)
			result *= base;
	}
	else
	{
		for (int i = 0; i < -exponent; i++)
			result *= 1.0 / base;
	}
	return result;
}
#endif

#ifdef BOOK
double Power(double base, int exponent)
{
	g_InvalidInput = false;

	if (equal(base, 0.0) && exponent < 0)
	{
		g_InvalidInput = true;
		return 0.0;
	}

	unsigned int absExponent = (unsigned int)(exponent>0?(exponent):(-exponent));

	double result = PowerWithUnsignedExponent(base, absExponent);
	if (exponent < 0)
		result = 1.0 / result;
	return result;
}

#endif

#ifndef NEW_SOLUTION
double PowerWithUnsignedExponent(double base, unsigned int exponent)
{
	double result = 1.0;
	for (int i = 0; i < exponent; i++)
		result *= base;
	return result;
}
#endif

#ifdef NEW_SOLUTION
double PowerWithUnsignedExponent(double base, unsigned int exponent)
{
	if (exponent == 0)
		return 1;
	if (exponent == 1)
		return base;
	double result = PowerWithUnsignedExponent(base, exponent >> 1);
	result *= result;
	if (exponent & 0x1 == 1)
		result *= base;
	
	return result;
}
#endif


bool equal(double num1, double num2)
{
	if ((num1 - num2 > -0.0000001)
		&& (num1 - num2 < 0.0000001))
		return true;
	else
		return false;
}

void Test(double base, int exponent, double expectedResult, bool expectedFlag)
{
	double result = Power(base, exponent);
	if (abs(result - expectedResult) < 0.00000001
		&& g_InvalidInput == expectedFlag)
		printf("Test passed.\n");
	else
		printf("Test failed.\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	// ������ָ����Ϊ����
	printf("Test1 begins.\n");
	Test(2, 3, 8, false);

	// ����Ϊ������ָ��Ϊ����
	printf("Test2 begins.\n");
	Test(-2, 3, -8, false);

	// ָ��Ϊ����
	printf("Test3 begins.\n");
	Test(2, -3, 0.125, false);

	// ָ��Ϊ0
	printf("Test4 begins.\n");
	Test(2, 0, 1, false);

	// ������ָ����Ϊ0
	printf("Test5 begins.\n");
	Test(0, 0, 1, false);

	// ����Ϊ0��ָ��Ϊ����
	printf("Test6 begins.\n");
	Test(0, 4, 0, false);

	// ����Ϊ0��ָ��Ϊ����
	printf("Test7 begins.\n");
	Test(0, -4, 0, true);

	cin.get();
	return 0;
}


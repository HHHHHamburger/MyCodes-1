// �ݹ�ʵ�������ַ������.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��дһ������������int�ͣ����������������ַ������磺����123��
//����"321"��Ҫ�����ʹ�õݹ飬������ȫ�ֱ��������������һ������,
//���뷵���ַ���
#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;


string ReversIntToString(int original)
{
	int unSignedOriginal = original >= 0 ? original : -original;
	string result = "";
	if (unSignedOriginal < 10)
		return result += unSignedOriginal + '0';
	result += unSignedOriginal % 10 + '0';
	result += ReversIntToString(unSignedOriginal / 10);
	if (original < 0)result += '-';
	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string s = "";
	s = ReversIntToString(-123);
	cout << s << endl;
	cin.get();
	return 0;
}


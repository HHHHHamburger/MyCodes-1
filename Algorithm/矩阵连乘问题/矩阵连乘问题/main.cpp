#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#pragma warning(disable:4996)

using namespace std;

const int SIZE = 7;

int s[SIZE][SIZE], m[SIZE][SIZE];

int MatrixChain(int n, int m[SIZE][SIZE], int s[SIZE][SIZE], int *p)
{
	for (int i = 1; i <= n; i++)
		m[i][i] = 0;

	for (int r = 2; r <= n; r++)//rΪ��ǰ������������������ģ�� 
	{
		for (int i = 1; i <= n - r + 1; i++)//n-r+1Ϊ���һ��r����ǰ�߽� 
		{
			int j = i + r - 1; // ����ǰ�߽�Ϊr������Ϊr�����ĺ�߽�

			m[i][j] = m[i][i] + m[i + 1][j] + p[i - 1] * p[i] * p[j];//����A[i:j]����ΪA(i) * ( A[i+1:j] )  

			s[i][j] = i;//����i���Ͽ�

			for (int k = i + 1; k < j; k++)
			{
				int t = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (t < m[i][j])
				{
					m[i][j] = t;
					s[i][j] = k;
				}
			}
		}
	}
	return m[1][SIZE - 1];
}

void Traceback(int i, int j, int s[SIZE][SIZE])
{
	if (i == j) return;
	Traceback(i, s[i][j], s);
	Traceback(s[i][j] + 1, j, s);
	cout << "Multiply A" << i << "," << s[i][j];	//A2,2 and A3,3��ʾA2*A3
	cout << " and A" << (s[i][j] + 1) << "," << j << endl;	//A1,1 and A2,3��ʾA1*(A2*A3)
}

int main()
{
	int p[SIZE] = { 30, 35, 15, 5, 10, 20, 25 };

	cout << "��������ټ������Ϊ��" << MatrixChain(6, m, s, p) << endl;
	cout << "�������ż������Ϊ��" << endl;
	Traceback(1, 6, s);
	return 0;

}
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;

const int SIZE = 1e3+10;
int A[SIZE][SIZE];


int MaxSum(int *s, int len)
{
	int sum = -1 << 30, b = 0;
	for (int i = 1; i <= len; i++)
	{
		b += s[i];	//��ʱb����b[i-1],Ϊi֮ǰ��Ӵ���
		if (b<s[i])	//��ʱb����b[i],Ϊi��i֮ǰ��Ӵ���,��s[i]�Ƚϴ�С
			b = s[i];
		if (b > sum)
		{
			sum = b;
		}
	}
	return sum;
}

int MaxSum2(int s[SIZE][SIZE], int n, int m)
{
	int sum = -1 << 30;
	int *b = new int[m+1];
	for (int i = 1; i <= n; i++)	//ö����
	{
		for (int k = 1; k <= m; k++)
			b[k] = 0;
		for (int j = i; j <= n; j++)	//�ӵ�i�п�ʼ����
		{
			for (int k = 1; k <= m; k++)
			{
				b[k] += s[j][k];	//b[k]Ϊ�ӵ�i�е���j�������k��֮��
				//���������ΰ������ʹ��MaxSum���һά�ռ������ֵ��Ϊ��ά�����ֵ
				int max = MaxSum(b, m);
				if (max > sum)
				{
					sum = max;
				}
			}
		}
	}
	return sum;
}

void Input(int s[SIZE][SIZE],int &r,int &c)
{
	int rows,cols;
	cout << "Enter rows:";
	cin >> rows;
	cout << "Enter cols:";
	cin >> cols;
	//len = rand() % 100 + 2;
	for (int i = 1; i <= rows; i++)
		for(int j=1;j<=cols;j++)
			s[i][j] = rand() % 100 - 50;
	//cin >> s[i];
	c = cols;
	r = rows;
}

void Output(int s[SIZE][SIZE], int r,int c)
{
	for (int i = 1; i <= r; i++)
	{
		for (int j = 1; j <= c; j++)
		{
			cout.width(3);
			cout << s[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{
	srand(time(0));
	int t = 500;
	while (t--)
	{
		int r, c;
		Input(A, r, c);
		if (r <= 0||c<=0)break;
		Output(A, r,c);
		cout << "Max Sum is:" << MaxSum2(A,r,c) << endl;
	}
	return 0;
}


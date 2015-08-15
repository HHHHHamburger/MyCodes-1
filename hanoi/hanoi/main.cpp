#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#pragma warning(disable:4996)

using namespace std;

void hanoi(int n,char a,char b,char c)
{
	if (n > 0)
	{
		hanoi(n - 1, a, c, b);//����b,��n-1�����Ӵ�a�Ƶ�c  
		printf(" %c �ƶ��� %c \n", a, b);
		hanoi(n - 1, c, a, b);
	}
}


int main()
{
	int T;
	scanf("%d", &T);
	for (int i = 1; i <= T; i++)
	{
		int n;
		scanf("%d", &n);
		hanoi(n,'a','b','c');
		printf("\n");
	}
	return 0;
}

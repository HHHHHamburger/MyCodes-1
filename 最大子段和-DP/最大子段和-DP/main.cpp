#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
const int SIZE = 1e5+10;
int A[SIZE];
using namespace std;

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


int Input(int *s)
{
	int len;
	cout << "Enter length:";
	cin >> len;
	//len = rand() % 100 + 2;
	for (int i = 1; i <= len; i++)
		s[i] = rand() % 100 - 50;
		//cin >> s[i];
	return len;
}

void Output(int *s, int len)
{
	cout << len<<endl;
	for (int i = 1; i <= len; i++)
	{
		cout << s[i] << " ";
	}
	cout << endl;
}

int main()
{
	srand(time(0));
	int t = 500;
	while (t--)
	{
		int len = Input(A);
		if (len <= 0)break;
		Output(A, len);
		cout << "Max Sum is:" << MaxSum(A, len) << endl;
	}
	return 0;
}


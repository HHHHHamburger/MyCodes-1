//��һ��¥����10��̨�ף��涨ÿһ��ֻ�ܿ�һ����������Ҫ���ϵ�10��̨���м��ֲ�ͬ���߷�?
#include <iostream>
using namespace std;

int f[11];


//f[i]�������i��̨�׵ķ�����
void dp(int i)
{
	if (i == 1)
	{
		f[i] = 1;
		return;
	}
	if (i == 2)
	{
		f[i] = 2;
		return;
	}
	if (f[i] != -1)return;
	f[i] = 0;
	dp(i - 1);
	dp(i - 2);
	f[i] += f[i - 1] + f[i - 2];
}

int main()
{
	int i;
	while (cin >> i&&i)
	{
		memset(f, -1, sizeof(f));
		dp(i);
		cout << f[i] << endl;
		cin.get();
	}
	return 0;
}
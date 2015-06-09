/*
��n�������ͼ�ֵ�ֱ�Ϊvector<int> weight, vector<int> value����Ʒ�����������ΪW�������ñ���װ�µ���Ʒ������ֵ��
���룺n =4
weight=2, 1, 3, 2
value =3, 2, 4, 2
W=5
���=7
*/
#include <iostream>
#include <vector>
#pragma warning(disable:4996)
using namespace std;


vector<int> w, v;
int n;
int W;
int dp[10][10];

int max(int a, int b)
{
	return a > b ? a : b;
}

int rec(int i, int j)	//�ݹ��㷨��O(n)=(2^n)
{
	//��i��n����Ʒ��ѡ������������j����Ʒ������ֵ  
	int res;
	if (i == n)
	{
		res = 0;
	}
	else if (j< w[i])
	{
		res = rec(i + 1, j);	//��ǰ��Ʒ��ֵ�������ޣ���ʣ����Ʒ��ѡ��
	}
	else
	{
		res = max(rec(i + 1, j), rec(i + 1, j - w[i]) + v[i]);
		//ѡ�񣨲�ѡ��ǰ��Ʒ��ʣ����Ʒ�м�ֵ���ģ���
		//��ѡ��ǰ��Ʒ����ʣ����Ʒ��ѡ������������)�����ֵ
	}
	return res;
}

void DP()	//dp[i][j]��ʾʣ��ǰi����Ʒ��ѡ��������j֮�ڵ�����ֵ
{
	for (int i = n-1; i >=0;i--)
	for (int j = 0; j <= W; j++)
	{
		if (j < w[i])
			dp[i][j] = dp[i + 1][j];
		else
			dp[i][j] = max(dp[i + 1][j], dp[i + 1][j - w[i]] + v[i]);
	}
}

void DP2()	//dp[i][j]��ʾ��ѡ����ǰi����Ʒ��������j֮�ڵ�����ֵ
{
	for (int i = 1; i <= n;i++)
	for (int j = 1; j <= W; j++)
	{
		if (j < w[i-1])
			dp[i][j] = dp[i - 1][j];
		else
			dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i-1]] + v[i-1]);
	}
}



int main()
{
	freopen("in.txt", "r", stdin);
	cin >> n;
	memset(dp, 0, sizeof(dp));
	int weight, value;
	for (int i = 1; i <= n; i++)
	{
		cin >> weight;
		w.push_back(weight);
	}
	for (int i = 1; i <= n; i++)
	{
		cin >> value;
		v.push_back(value);
	}
	cin >> W;
	int ans;
	DP2();
	ans = dp[n][W];
	cout << ans << endl;
	cin.get();
	return 0;
}

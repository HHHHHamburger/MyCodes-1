//͹��������������ʷ�
#include <stack>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>

using namespace std;

const int N = 7;//͹����α���+1
int s[N][N], t[N][N];
int weight[][N] = { { 0, 2, 2, 3, 1, 4 }, { 2, 0, 1, 5, 2, 3 }, { 2, 1, 0, 2, 1, 4 }, { 3, 5, 2, 0, 6, 2 }, { 1, 2, 1, 6, 0, 1 }, { 4, 3, 4, 2, 1, 0 } };//͹����ε�Ȩ

int MinWeightTriangulation(int n, int t[][N], int s[][N]);
void Traceback(int i, int j, int s[][N]);//�������Ž�
int Weight(int a, int b, int c);//Ȩ����
void Traceback_norecursively(int i, int j, int s[][N]);

int main()
{
	cout << "�˶���ε����������ʷ�ֵΪ��" << MinWeightTriangulation(N - 1, t, s) << endl;
	cout << "���������ʷֽṹΪ��" << endl;
	Traceback(1, 5, s); //s[i][j]��¼��Vi-1��Vj���������εĵ�3�������λ��
	cout << "���������ʷֽṹΪ(�ǵݹ�)��" << endl;
	Traceback_norecursively(1, 5, s);
	cin.get();
	return 0;
}

int MinWeightTriangulation(int n, int t[][N], int s[][N])
{
	for (int i = 1; i <= n; i++)
	{
		t[i][i] = 0;
	}

	for (int r = 2; r <= n; r++)//rΪ��ǰ������������������ģ��
	{
		for (int i = 1; i <= n - r + 1; i++)//n-r+1Ϊ���һ��r����ǰ�߽�
		{
			int j = i + r - 1;//����ǰ�߽�Ϊi������Ϊr�����ĺ�߽�

			//����Vi-1ViVj���������ʷ֣���ʼ��
			t[i][j] = t[i][i] + t[i + 1][j] + Weight(i - 1, i, j);//����ij����ΪA(i) * ( A[i+1:j] )����ʵ���Ͼ���k=i  
					//t[i][i]=0;
			s[i][j] = i;

			//������i~j�������ʷ�t[i][j];
			for (int k = i + 1; k < j; k++)
			{
				int min = t[i][k] + t[k + 1][j] + Weight(i - 1, k, j);//����Vi-1VkVj�ʷ֣����뵱ǰֵ���бȽ�

				if (min < t[i][j])
				{
					t[i][j] = min;
					s[i][j] = k;
				}
			}
		}
	}
	return t[1][N - 2];
}


void Traceback_norecursively(int i, int j, int s[][N])
{
	struct POS
	{
		int i, j;
		POS(int I,int J)
		{
			i = I;
			j = J;
		}
	}tmp(-1,-1);
	stack<POS> s1,s2;
	s1.push(POS(i,j));
	while (!s1.empty())  // ջ��ʱ����       
	{
		tmp = s1.top();
		s1.pop();
		s2.push(tmp);
		if (tmp.i!=s[tmp.i][tmp.j])
			s1.push(POS(tmp.i, s[tmp.i][tmp.j]));
		if (s[tmp.i][tmp.j] + 1!=tmp.j)
			s1.push(POS(s[tmp.i][tmp.j] + 1, tmp.j));
	}
	while (!s2.empty())
	{
		tmp = s2.top();
		cout << "�����ʷֶ��㣺V" << tmp.i - 1 << ",V" << tmp.j << ",V" << s[tmp.i][tmp.j] << endl;
		s2.pop();
	}
}

void Traceback(int i, int j, int s[][N])
{
	if (i == j) return;
	Traceback(i, s[i][j], s);
	Traceback(s[i][j] + 1, j, s);
	cout << "�����ʷֶ��㣺V" << i - 1 << ",V" << j << ",V" << s[i][j] << endl;
}

int Weight(int a, int b, int c)
{
	return weight[a][b] + weight[b][c] + weight[a][c];
}
//2d11 ���η���ѭ�������ճ̱�
#include <iostream>    
#include <math.h>
using namespace std;

void Table(int k, int n, int **a);
void input(int &k);
void output(int **a, int n);

int main()
{
	int k;
	input(k);

	int n = 1;
	//n=2k(k>=1)��ѡ�ֲμӱ���
	for (int i = 1; i <= k; i++)
		n *= 2;

	//����n��̬�����ά����a
	int **a = new int *[n + 1];
	for (int i = 0; i <= n; i++)
	{
		a[i] = new int[n + 1];
	}


	Table(k, n, a);

	cout << "ѭ�������ճ̱�Ϊ��" << endl;
	output(a, n);

	//�ͷſռ�
	for (int i = 0; i <= n; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	cin.get();
	cin.get();
	return 0;
}

void input(int &k)
{
	cout << "������kֵ��" << endl;
	cin >> k;
}

void output(int **a, int n)
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}

void Table(int k, int n, int **a)
{
	for (int i = 1; i <= n; i++)
		a[1][i] = i;//�����ճ̱��һ��

	int m = 1;//ÿ�����ʱ����ʼ���λ��
	for (int s = 1; s <= k; s++)
	{
		n /= 2;
		for (int t = 1; t <= n; t++)
		{
			for (int i = m + 1; i <= 2 * m; i++)//������
			{
				for (int j = m + 1; j <= 2 * m; j++)//������
				{
					a[i][j + (t - 1)*m * 2] = a[i - m][j + (t - 1)*m * 2 - m];//���½ǵ������Ͻǵ�ֵ
					a[i][j + (t - 1)*m * 2 - m] = a[i - m][j + (t - 1)*m * 2];//���½ǵ������Ͻǵ�ֵ
				}

			}
		}
		m *= 2;
	}
}
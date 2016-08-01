#include <iostream>
#include <cstring>
#pragma warning(disable:4996)
using namespace std;
char s1[33];
char s2[33];
char ans[33];
void build(int n, char* pre, char* in, char* post)
{
	if (n <= 0) return;
	int index = strchr(in, pre[0]) - in;	//�������������е�һ���ַ������������г��ֵ�λ��
	build(index, pre + 1, in, post);	//�Խ�������������ʹ�ú�������
	build(n - index - 1, pre + index + 1, in + index + 1, post + index);//�Խ�������������ʹ�ú�������
	post[n - 1] = pre[0];	//��Ӹý����������������
}
int main()
{
	while (cin >> s1 >> s2)
	{
		int n = strlen(s1);
		build(n, s1, s2, ans);
		ans[n] = '\0';
		cout << ans << endl;
	}
	return 0;
}
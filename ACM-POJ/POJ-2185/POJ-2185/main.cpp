#include<cstdio>
#include<cstring>
#pragma warning(disable:4996)
char s[10010][80];
int p[10010];
int flag[80];
char tmp[80];
int main()
{
	int x, y, r, c, i, j;
	while (scanf("%d%d", &r, &c) != EOF)
	{
		memset(flag, 0, sizeof(flag));
		for (i = 1; i <= r; i++)
		{
			scanf("%s", s[i] + 1);
			strcpy(tmp + 1, s[i] + 1);
			for (j = c - 1; j > 0; j--)
			{
				tmp[j + 1] = 0;
				for (x = 1, y = 1; s[i][y]!='\0'; x++, y++)
				{
					if (!tmp[x]) x = 1;
					if (tmp[x] != s[i][y]) break;
				}
				if (!s[i][y]) flag[j]++;
			}
		}
		for (j = 0; j<c; j++)
			if (flag[j] == r) 
				break;
		x = j;//��С�ظ��Ӿ��������

		for (i = 1; i <= r; i++)
			s[i][x + 1] = 0;

		p[1] = 0;//��������KMP��next������������С�ظ��Ӿ��������
		for (i = 2, j = 0; i <= r; i++)
		{
			while (j>0 && strcmp(s[j + 1] + 1, s[i] + 1)) 
				j = p[j];
			if (!strcmp(s[j + 1] + 1, s[i] + 1))
				j += 1;
			p[i] = j;
		}
		printf("%d\n", x*(r - p[r]));//������˼�Ϊ���ս��
	}
}
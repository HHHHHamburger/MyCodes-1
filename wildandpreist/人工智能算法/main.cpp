/*
�����о�3��Ұ�˺�3����ʦ���ӵ�����
�������£�
С��ÿ������������ˣ�û�������޷�����
����ʱ������Ӱ�����һ��Ұ������������ʦ����������Ϸʧ��

�������ݣ�
����DFS(��������������㷨������Ұ�����ȹ��ӵ�ԭ��
��˳��ֱ����2Ұ�ˡ�1Ұ��1��ʦ��1Ұ�ˡ�1��ʦ��2��ʦ�Ĺ������ȼ�
�����жϵ�ǰ����Ƿ�˳����������һ��������жϹ����Ժ�����������������Ϸ���
�����Ϸ����������һ������ĳ���
���Ϸ����򽫸ý����������������
Ȼ����õݹ飬������չ�Ľ��������ϲ���
ֱ���õ�3��Ұ�˺�3����ʦ���ɹ����ӵĽ�����У����������õ��жϱ�����ΪTRUE(1)���˳�����ѭ��
����������ӵķ���
�������
*/
#include <iostream>
using namespace std;

#define N 3

struct riverside
{
	int wildman;
	int priest;
};

struct Boat
{
	int side;//0����ǰ�����󰶣�1Ϊ�Ұ�
	int wildman;
	int priest;
};

struct status
{
	riverside rightside;
	riverside leftside;
	Boat boat;              //����λ�ô���ǰλ�ã���һ���ƶ����ƶ����԰�������������һ���ƶ�ʱ��Ҫ�ƶ�����ʦ��Ұ������
	status *prev;           //ָ��ǰ����ǰһ���
	status *next;           //ָ��ǰ���ĺ�һ���			
};

Boat boatman[5], lastboat;
bool process(status *);
bool thesame(status *);
bool nextmove(status *);
void setstatus();

int flag = 0;          //�жϱ������ж��Ƿ���ɹ��Ӳ���
status *head = new status;



int main()
{
	setstatus();
	head->rightside.wildman = 0;
	head->rightside.priest = 0;
	head->leftside.priest = N;
	head->leftside.wildman = N;
	head->prev = nullptr;
	head->next = nullptr;
	head->boat.side = 0;
	head->boat.priest = 0;
	head->boat.wildman = 0;
	process(head);
	status *p;
	p = head;
	cout << "�����������:" << endl;
	do
	{
		cout << " ��Ұ��:" << p->leftside.wildman << " ����ʦ:" << p->leftside.priest
			<< " �Ұ�Ұ��:" << p->rightside.wildman << " �Ұ���ʦ:" << p->rightside.priest << "   ��һ��������";
		if (p->boat.side == 0)cout << "��->�Ұ�";
		else cout << "��<-�Ұ�";
		cout << endl;
		p = p->next;
	} while (p != nullptr);
	cout << "�����ϣ�";
	cin.get();
	return 0;
}

void setstatus()       //��ȫ�ֱ�����С���ƶ���Ա��5����������г�ʼ��
{
	boatman[0].wildman = 2;
	boatman[0].priest = 0;
	boatman[1].wildman = 1;
	boatman[1].priest = 1;
	boatman[2].wildman = 1;
	boatman[2].priest = 0;
	boatman[3].wildman = 0;
	boatman[3].priest = 1;
	boatman[4].wildman = 0;
	boatman[4].priest = 2;
	for (int i = 0; i < 5; i++)
		boatman[i].side = 0;
	lastboat.side = 0;
}


bool process(status *node)
{
	int i;
	if (node->rightside.wildman == N && node->rightside.priest == N)flag = 1;
	for (i = 0; i < 5; i++)
	{
		node->boat.wildman = boatman[i].wildman;
		node->boat.priest = boatman[i].priest;
		if (nextmove(node))
			process(node->next);
		if (flag == 1)return true;
	}
	return false;
}

bool thesame(status *p, status *node)
{
	for (p; p != node; p = p->next)
	if ((p->leftside.priest == node->leftside.priest) && (p->leftside.wildman == node->leftside.wildman) && (p->rightside.priest == node->rightside.priest) &&
		(p->rightside.wildman == node->rightside.wildman) && (p->boat.priest == node->boat.side) &&
		(p->boat.wildman == node->boat.wildman) && (p->boat.side == node->boat.side))return false;
	return true;
}

bool nextmove(status *node)
{
	//�жϵ�ǰ����Ƿ���Խ����ƶ�����һ���Ϸ����
	status *p;
	p = head;
	if (thesame(p, node) == false)return false;      //��֤��ǰ����ڽ��������δ������
	if (lastboat.priest == node->boat.priest&&lastboat.wildman == node->boat.wildman)return false;  //��֤�������Ķ�����ոշ����Ķ�������ͬ
	if (node->boat.side == 1)
	{
		if ((node->rightside.priest - node->boat.priest) < 0)return false;  //��ִ֤�ж�����ǰ���������ڵ���0
		if ((node->rightside.wildman - node->boat.wildman) < 0)return false; //��ִ֤�ж�����ǰ���������ڵ���0
		if (((node->rightside.priest - node->boat.priest)<(node->rightside.wildman - node->boat.wildman)) &&
			(node->rightside.priest - node->boat.priest)>0)return false;  //��֤��ǰ��ִ�ж���������ʦ������Ϊ0ʱ��ʦ�������ڵ���Ұ������ 
		if ((((node->leftside.priest + node->boat.priest)<(node->leftside.wildman + node->boat.wildman))) &&
			(node->leftside.priest + node->boat.priest)>0)return false;   //��֤�԰�ִ�ж���������ʦ������Ϊ0ʱ��ʦ�������ڵ���Ұ������
	}
	else
	{
		if ((node->leftside.priest - node->boat.priest) < 0)return false;          //ͬ��
		if ((node->leftside.wildman - node->boat.wildman) < 0)return false;	          //ͬ��
		if ((((node->leftside.priest - node->boat.priest)<(node->leftside.wildman - node->boat.wildman))) &&
			(node->leftside.priest - node->boat.priest)>0)return false;          //ͬ��
		if (((node->rightside.priest + node->boat.priest)<(node->rightside.wildman + node->boat.wildman)) &&
			(node->rightside.priest + node->boat.priest)>0)return false;          //ͬ��
	}
	//���������������������չ��ǰ��㣬�����ƶ�
	status *nodenext = new status;
	node->next = nodenext;
	if (node->boat.side == 0)
	{
		nodenext->rightside.priest = node->rightside.priest + node->boat.priest;
		nodenext->rightside.wildman = node->rightside.wildman + node->boat.wildman;
		nodenext->leftside.priest = node->leftside.priest - node->boat.priest;
		nodenext->leftside.wildman = node->leftside.wildman - node->boat.wildman;
		nodenext->prev = node;
		nodenext->next = nullptr;
		nodenext->boat.side = 1;
		nodenext->boat.priest = nodenext->boat.wildman = 0;
		lastboat.priest = node->boat.priest;
		lastboat.wildman = node->boat.wildman;
	}
	else
	{
		nodenext->rightside.priest = node->rightside.priest - node->boat.priest;
		nodenext->rightside.wildman = node->rightside.wildman - node->boat.wildman;
		nodenext->leftside.priest = node->leftside.priest + node->boat.priest;
		nodenext->leftside.wildman = node->leftside.wildman + node->boat.wildman;
		nodenext->prev = node;
		nodenext->next = nullptr;
		nodenext->boat.side = 0;
		nodenext->boat.priest = nodenext->boat.wildman = 0;
		lastboat.priest = node->boat.priest;
		lastboat.wildman = node->boat.wildman;
	}
	return true;
}
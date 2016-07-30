// �����е�����k�����.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:����һ����������������е�����k����㡣Ϊ�˷��ϴ������
//��ϰ�ߣ������1��ʼ�������������β���ʱ������1����㡣����һ
//��������6����㣬��ͷ��㿪ʼ���ǵ�ֵһ����1��2��3��4��5��6����
//������ĵ�����3�����ʱֵΪ4�Ľ��
#include "stdafx.h"
#include <iostream>

struct ListNode
{
	int       m_nValue;
	ListNode* m_pNext;
};
using namespace std;

void PrintListNode(ListNode* pNode)
{
	if (pNode == NULL)
	{
		printf("The node is NULL\n");
	}
	else
	{
		printf("The key in node is %d.\n", pNode->m_nValue);
	}
}

ListNode* CreateListNode(int value)
{
	ListNode* pNode = new ListNode();
	pNode->m_nValue = value;
	pNode->m_pNext = NULL;

	return pNode;
}

void ConnectListNodes(ListNode* pCurrent, ListNode* pNext)
{
	if (pCurrent == NULL)
	{
		printf("Error to connect two nodes.\n");
		exit(1);
	}

	pCurrent->m_pNext = pNext;
}

void DestroyList(ListNode* pHead)
{
	ListNode* pNode = pHead;
	while (pNode != NULL)
	{
		pHead = pHead->m_pNext;
		delete pNode;
		pNode = pHead;
	}
}

ListNode* PrintTheLastKthNode(ListNode *head,int k)
{
	if (head == NULL || k <= 0)
		return NULL;
	ListNode *pAhead = head;
	ListNode *pBehind = NULL;
	int i;
	for (i = 0; i < k - 1 && pAhead->m_pNext != NULL; i++)
	{
		pAhead = pAhead->m_pNext;
	}
	if (i != k - 1)return pBehind;
	pBehind = head;
	while (pAhead->m_pNext != NULL)
	{
		pAhead = pAhead->m_pNext;
		pBehind = pBehind->m_pNext;
	}
	return pBehind;
}
void Test1()
{
	printf("=====Test1 starts:=====\n");
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	printf("expected result: 4.\n");
	ListNode* pNode = PrintTheLastKthNode(pNode1, 2);
	PrintListNode(pNode);

	DestroyList(pNode1);
}

// ����Ҫ�ҵĽ���������β���
void Test2()
{
	printf("=====Test2 starts:=====\n");
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	printf("expected result: 5.\n");
	ListNode* pNode = PrintTheLastKthNode(pNode1, 1);
	PrintListNode(pNode);

	DestroyList(pNode1);
}

// ����Ҫ�ҵĽ���������ͷ���
void Test3()
{
	printf("=====Test3 starts:=====\n");
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	printf("expected result: 1.\n");
	ListNode* pNode = PrintTheLastKthNode(pNode1, 5);
	PrintListNode(pNode);

	DestroyList(pNode1);
}

// ���Կ�����
void Test4()
{
	printf("=====Test4 starts:=====\n");
	printf("expected result: NULL.\n");
	ListNode* pNode = PrintTheLastKthNode(NULL, 100);
	PrintListNode(pNode);
}

// ��������ĵڶ���������������Ľ������
void Test5()
{
	printf("=====Test5 starts:=====\n");
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	printf("expected result: NULL.\n");
	ListNode* pNode = PrintTheLastKthNode(pNode1, 6);
	PrintListNode(pNode);

	DestroyList(pNode1);
}

// ��������ĵڶ�������Ϊ0
void Test6()
{
	printf("=====Test6 starts:=====\n");
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	printf("expected result: NULL.\n");
	ListNode* pNode = PrintTheLastKthNode(pNode1, 0);
	PrintListNode(pNode);

	DestroyList(pNode1);
}


int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	cin.get();
	return 0;
}


// ��O(1)ʱ��ɾ��������.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:�������������ͷָ���һ�����ָ�룬����һ��������O(1)
//ʱ��ɾ���ý��
#include "stdafx.h"
#include <iostream>
using namespace std;

struct ListNode
{
	int m_nValue;
	ListNode *m_pNext;
};

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

void PrintList(ListNode* pHead)
{
	printf("PrintList starts.\n");

	ListNode* pNode = pHead;
	while (pNode != NULL)
	{
		printf("%d\t", pNode->m_nValue);
		pNode = pNode->m_pNext;
	}

	printf("\nPrintList ends.\n");
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

void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted)
{
	if (*pListHead == NULL||pToBeDeleted==NULL)
		return;
	if (*pListHead == pToBeDeleted)  //���ɾ��ͷ���
	{
		if (pToBeDeleted->m_pNext == NULL)	//ֻ��һ�����
		{
			*pListHead = NULL;
			return;
		}
		else
		{
			(*pListHead) = pToBeDeleted->m_pNext;
			return;
		}
	}
	else if (pToBeDeleted->m_pNext != NULL)
	{
		ListNode *node = pToBeDeleted->m_pNext;
		pToBeDeleted->m_nValue = node->m_nValue;
		pToBeDeleted->m_pNext = node->m_pNext;
		delete node;
		node = NULL;
		return;
	}
	else
	{
		ListNode *node = *pListHead;
		while (node->m_pNext != pToBeDeleted)
			node = node->m_pNext;
		node->m_pNext = NULL;
		delete pToBeDeleted;
		pToBeDeleted = NULL;
		return;
	}
}

void Test(ListNode* pListHead, ListNode* pNode)
{
	printf("The original list is: \n");
	PrintList(pListHead);

	printf("The node to be deleted is: \n");
	PrintListNode(pNode);

	DeleteNode(&pListHead, pNode);

	printf("The result list is: \n");
	PrintList(pListHead);
}

// �������ж����㣬ɾ���м�Ľ��
void Test1()
{
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	Test(pNode1, pNode3);

	DestroyList(pNode1);
}

// �������ж����㣬ɾ��β���
void Test2()
{
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	Test(pNode1, pNode5);

	DestroyList(pNode1);
}

// �������ж����㣬ɾ��ͷ���
void Test3()
{
	ListNode* pNode1 = CreateListNode(1);
	ListNode* pNode2 = CreateListNode(2);
	ListNode* pNode3 = CreateListNode(3);
	ListNode* pNode4 = CreateListNode(4);
	ListNode* pNode5 = CreateListNode(5);

	ConnectListNodes(pNode1, pNode2);
	ConnectListNodes(pNode2, pNode3);
	ConnectListNodes(pNode3, pNode4);
	ConnectListNodes(pNode4, pNode5);

	Test(pNode1, pNode1);

	DestroyList(pNode1);
}

// ������ֻ��һ����㣬ɾ��ͷ���
void Test4()
{
	ListNode* pNode1 = CreateListNode(1);

	Test(pNode1, pNode1);
}

// ����Ϊ��
void Test5()
{
	Test(NULL, NULL);
}


int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	cin.get();
	return 0;
}


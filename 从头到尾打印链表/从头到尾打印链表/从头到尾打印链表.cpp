// ��ͷ��β��ӡ����.cpp : �������̨Ӧ�ó������ڵ㡣
//
//��Ŀ:����һ�������ͷ��㣬��β��ͷ��������ӡ��ÿ������ֵ
#include "stdafx.h"
#include <iostream>
#include <stack>
using namespace std;

struct ListNode
{
	int Key;
	ListNode* Next;
};

void AddToTail(ListNode** pHead, int value)  //��ֵ���������
{
	ListNode* pNew = new ListNode();
	pNew->Key = value;
	pNew->Next = NULL;

	if (*pHead == NULL)
	{
		*pHead = pNew;
	}
	else
	{
		ListNode* pNode = *pHead;

		while (pNode->Next != NULL)
			pNode = pNode->Next;

		pNode->Next = pNew;
	}
}

//ʹ��ջ�ķ���
void PrintListReversingly_stack(ListNode * pHead)
{
	stack<ListNode *> nodes;
	if (pHead == NULL)return;
	ListNode* pNode = pHead;
	while (pNode != NULL)
	{
		nodes.push(pNode);
		pNode = pNode->Next;
	}
	while (!nodes.empty())
	{
		pNode = nodes.top();
		cout << pNode->Key << " ";
		nodes.pop();
	}
}

//ʹ�õݹ鷽��
void PrintListReversingly_Recursive(ListNode * pHead)
{
	if (pHead == NULL)return;
	else
	{
		PrintListReversingly_Recursive(pHead->Next);
		cout << pHead->Key << " ";
	}
}

// 1->2->3->4->5
void Test1()
{
	printf("\nTest1 begins.\n");

	ListNode* pHead=NULL;
	AddToTail(&pHead, 1);
	AddToTail(&pHead, 2);
	AddToTail(&pHead, 3);
	AddToTail(&pHead, 4);
	AddToTail(&pHead, 5);
	PrintListReversingly_stack(pHead);
	cout << endl;
	PrintListReversingly_Recursive(pHead);
}

// ֻ��һ����������: 1
void Test2()
{
	printf("\nTest2 begins.\n");

	ListNode* pHead = NULL;
	AddToTail(&pHead, 1);

	PrintListReversingly_stack(pHead);
	cout << endl;
	PrintListReversingly_Recursive(pHead);
}

// ������
void Test3()
{
	printf("\nTest3 begins.\n");

	PrintListReversingly_stack(NULL);
	cout << endl;
	PrintListReversingly_Recursive(NULL);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	Test2();
	Test3();
	cin.get();
	return 0;
}


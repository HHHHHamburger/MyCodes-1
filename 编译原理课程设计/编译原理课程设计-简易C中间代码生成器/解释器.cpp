

#include "stdafx.h"

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stack>
#include<map>
#include "����ԭ��γ����-����C�м����������.h"
#include "����ԭ��γ����-����C�м����������Dlg.h"
using namespace std;
#define CodeMaxLength 1000

map<int, status> vars;//����map�ṹ

stack<double> numstack;//���������ջ

int VirtualMachine() {
	int i = 0;
	int StackTopValue;
	int content;
	int operator1;
	int operator2;
	int Code[CodeMaxLength][2];//0������룬1���������CodeMaxLength������Ҫȷ��
	while (i<CodeMaxLength)
	{
		switch (Code[i][0]) {
		case 0://Push d
			content = vars[Code[i][1]].value;
			numstack.push(content);
			i++;
			break;
		case 1://Pushi N
			content = Code[i][1];
			numstack.push(content);
			i++;
			break;
		case 2://Sto d
			content = numstack.top();
			vars[Code[i][1]].value = content;
			i++;
			break;
		case 3://Sti d
			content = numstack.top();
			vars[Code[i][1]].value = content;
			numstack.pop();
			i++;
			break;
		case 4://Add
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1 + operator2);
			i++;
			break;
		case 5://Sub
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1 - operator2);
			i++;
		case 6://Mul
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1 * operator2);
			i++;
		case 7://Div
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1 / operator2);
			i++;
		case 8://jmp
			i++;
			break;
		case 9://jnz
			i++;
			break;
		case 10://jmpz
			i++;
			break;
		case 11://Eq
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 == operator2)
				numstack.push(1);
			else numstack.push(0);
			i++;
			break;
		case 12://Noteq
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 == operator2)
				numstack.push(0);
			else numstack.push(1);
			i++;
			break;
		case 13://Gt
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 < operator2)
				numstack.push(1);
			else numstack.push(0);
			i++;
			break;
		case 14://Ls
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 > operator2)
				numstack.push(1);
			else numstack.push(0);
			i++;
			break;
		case 15://Geq
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 <= operator2)
				numstack.push(1);
			else numstack.push(0);
			i++;
			break;
		case 16://Leq
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			if (operator1 >= operator2)
				numstack.push(1);
			else numstack.push(0);
			i++;
			break;
		case 17://And
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1&&operator2);
			i++;
			break;
		case 18://Or
			operator1 = numstack.top();
			numstack.pop();
			operator2 = numstack.top();
			numstack.pop();
			numstack.push(operator1||operator2);
			i++;
			break;
		case 19://Not
			operator1 = numstack.top();
			numstack.pop();
			numstack.push(!operator1);
			i++;
			break;
		case 20://In
			cout << "Please Input " << vars[Code[i][1]].name << "'value:";
			cin >> operator1;
			vars[Code[i][1]].value = operator1;
			i++;
			break;
		case 21://Out
			operator1 = numstack.top();
			numstack.pop();
			cout << "The result is:" << operator1 << endl;;
			i++;
			break;
		case 22://Stop
			i = CodeMaxLength;
			break;
		default:
			i++;
			break;

		}

		return erroFlag;//�������ͱ�ʶ��erroFlag�趨�壬����ֱ���޸ĺ���������Ϊvoid��ɾ�������
}

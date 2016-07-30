#include "STATUS.h"
#include "stdafx.h"

STATUS::STATUS()
{
	on=false;		
	mode=0;		
	cooling = true;		//����/����
	sleep = false;			//˯��
	time = -1;		//��ʱ
	temperature = 26;		//�¶�
	rate = 0;			//����
	
}

STATUS::~STATUS()
{

}

void STATUS::setPointer(LPVOID pParam)
{
	pointer = (CTCPͨ�ſյ��ն�MFCDlg *)pParam;
}

void STATUS::checkInstrucion(char *ins,int val)
{
	pointer->SetDlgItemText(IDC_TEST, CString(ins));
	if (!strcmp(ins, "POWER"))
	{
		setOn();
		return;
	}
	if (!strcmp(ins, "MODE"))
	{
		setMode(val);
		return;
	}
	if (!strcmp(ins, "RATE"))
	{
		setRate(val);
		return;
	}
	if (!strcmp(ins, "SLEEP"))
	{
		setSleep();
		return;
	}
	if (!strcmp(ins, "TIME"))
	{
		setTime(val);
		return;
	}
	if (!strcmp(ins, "TEMPERATURE"))
	{
		setTemperature(val);
		return;
	}
}

void STATUS::Reset()
{
	mode = 0;
	cooling = true;		//����/����
	sleep = false;			//˯��
	time = -1;		//��ʱ
	temperature = 26;		//�¶�
	rate = 0;			//����
}

void STATUS::setOn()
{
	on=on == true ? false : true;
	if (on)Reset();
	setAirconditioner();
}
void STATUS::setMode(int m)
{
	if (!on)return;
	if (mode == m || m<0 || m>2)return;
	mode = m;
	setAirconditioner();
}
void STATUS::setCooling()
{
	if (!on)return;
	cooling=cooling == true ? false : true;
	setAirconditioner();
}
void STATUS::setSleep()
{
	if (!on)return;
	sleep=sleep == true ? false : true;
	setAirconditioner();
}
void STATUS::setTime(int c)
{
	if (!on)return;
	if (c < -1)return;
	time = c;
	setAirconditioner();
}
void STATUS::setTemperature(int t)
{
	if (!on)return;
	int tmp = temperature + t;
	if (tmp<10 || tmp>36)return;
	temperature = tmp;
	setAirconditioner();
}
void STATUS::setRate(int r)
{
	if (!on)return;
	if (rate == r || r<0 || r>3)return;
	rate = r;
	setAirconditioner();
}
void STATUS::setAirconditioner()
{
	static const char* MODES[3]{"�Զ�", "����", "����"};
	static const char* RATES[4]{"�Զ�", "�ͷ�", "�з�", "�߷�"};
	if (on)
	{
		CString temp,temptime;
		temp.Format(_T("%d"),temperature);
		temptime.Format(_T("%d"), time);
		pointer->SetDlgItemText(IDC_ONOFF, CString("������"));
		pointer->SetDlgItemText(IDC_TEMPRATURE, temp);
		pointer->SetDlgItemText(IDC_MODE, CString(MODES[mode]));
		pointer->SetDlgItemText(IDC_TIME, time == -1 ? CString("�ر�") : temptime);
		pointer->SetDlgItemText(IDC_SLEEP, sleep==false?CString("�ر�"):CString("����"));
		pointer->SetDlgItemText(IDC_RATE, CString(RATES[rate]));
	}
	else
	{
		pointer->SetDlgItemText(IDC_ONOFF, CString("������"));
		pointer->SetDlgItemText(IDC_TEMPRATURE, CString("������"));
		pointer->SetDlgItemText(IDC_MODE, CString("������"));
		pointer->SetDlgItemText(IDC_TIME, CString("������"));
		pointer->SetDlgItemText(IDC_SLEEP, CString("������"));
		pointer->SetDlgItemText(IDC_RATE, CString("������"));
	}
}
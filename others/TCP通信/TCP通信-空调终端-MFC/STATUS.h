#include "afxdialogex.h"
#include "TCPͨ��-�յ��ն�-MFC.h"
#include "TCPͨ��-�յ��ն�-MFCDlg.h"
#include <stdlib.h>



class STATUS
{
private:

	bool on;		//����״̬
	int mode;		//ģʽ
	bool cooling;		//����/����
	bool sleep;			//˯��
	int time;		//��ʱ
	int temperature;		//�¶�
	int rate;			//����
	CTCPͨ�ſյ��ն�MFCDlg * pointer;
public:
	STATUS();
	~STATUS();
	void setPointer(LPVOID pParam);
	void Reset();
	void checkInstrucion(char *ins,int val);
	void setOn();
	void setMode(int m);
	void setCooling();
	void setSleep();
	void setTime(int c);
	void setTemperature(int t);
	void setRate(int r);
	void setAirconditioner();
};
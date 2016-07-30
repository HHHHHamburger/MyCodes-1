
// TCPͨ��-�յ��ն�-MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCPͨ��-�յ��ն�-MFC.h"
#include "TCPͨ��-�յ��ն�-MFCDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdlib.h>
#include <cctype>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define MAXDATASIZE 100 
#define MYPORT 6667  /*�����û����Ӷ˿�*/ 
#define BACKLOG 10  /*���ٵȴ����ӿ���*/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char ins[20];//���ָ��
int SPACEPOS = -1;
static STATUS status;

UINT CTCPͨ�ſյ��ն�MFCDlg::ThreadProc(LPVOID pParam)
{
	int sockfd, new_fd;                                  /*�����׽���*/
	struct sockaddr_in my_addr;          /*���ص�ַ��Ϣ */
	struct sockaddr_in their_addr;        /*�����ߵ�ַ��Ϣ*/
	int sin_size, numbytes;   char msg[10], buf[MAXDATASIZE];
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);           //��ʼ��Windows Socket Dll
	//����socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		//�������socketʧ�ܣ��˳�����
		//printf("socket error\n");
		exit(1);
	}
	bool optval = true;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(bool));
	//bind������MYPORT�˿�
	my_addr.sin_family = AF_INET;                     /* Э��������INET  */
	my_addr.sin_port = htons(MYPORT);            /* ��MYPORT�˿�*/
	my_addr.sin_addr.s_addr = INADDR_ANY;    /* ����IP*/
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		//bindʧ�ܣ��˳�����
		//printf("bind error\n");
		closesocket(sockfd);
		exit(1);
	}


	//listen�������˿�
	//printf("listen...\n");
	int ret;
	int size = sizeof(struct sockaddr);
	while (1)
	{
		//	if ((numbytes = recv(new_fd, buf, MAXDATASIZE, 0)) == -1)  continue;
		ret = recvfrom(sockfd, buf, MAXDATASIZE, 0, (SOCKADDR*)&my_addr, &size);
		//Instructions(pParam,buf, sizeof(buf));
		SPACEPOS = -1;
		while (!(buf[++SPACEPOS] == ' '||buf[SPACEPOS] == '\0'));
		strcpy(ins, buf);
		ins[SPACEPOS] = '\0';
		status.checkInstrucion(ins, atoi(buf + SPACEPOS + 1));
		//printf("\n");
	}
	return 0;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTCPͨ�ſյ��ն�MFCDlg �Ի���



CTCPͨ�ſյ��ն�MFCDlg::CTCPͨ�ſյ��ն�MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPͨ�ſյ��ն�MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPͨ�ſյ��ն�MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTCPͨ�ſյ��ն�MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedInit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ON, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnOn)
	ON_BN_CLICKED(IDC_BTN_RATE, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnRate)
	ON_BN_CLICKED(IDC_BTN_TIME, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnTime)
	ON_BN_CLICKED(IDC_BTN_SLEEP2, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnSleep2)
	ON_BN_CLICKED(IDC_BTN_UP, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_BTN_MODE, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnMode)
	ON_BN_CLICKED(IDC_BTN_ON, &CTCPͨ�ſյ��ն�MFCDlg::OnClickedBtnOn)
	ON_BN_CLICKED(IDC_BTN_POWER, &CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnPower)
END_MESSAGE_MAP()


// CTCPͨ�ſյ��ն�MFCDlg ��Ϣ�������

BOOL CTCPͨ�ſյ��ն�MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//OnBnClickedInit();


	status.setPointer((LPVOID)this);
	status.setAirconditioner();
	Thread1 = AfxBeginThread(ThreadProc, (LPVOID)this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTCPͨ�ſյ��ն�MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTCPͨ�ſյ��ն�MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTCPͨ�ſյ��ն�MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedInit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_ONOFF, CString("������"));
	SetDlgItemText(IDC_TEMPRATURE, CString("26"));
	SetDlgItemText(IDC_TIME, CString("��"));
	SetDlgItemText(IDC_MODE, CString("����"));
	SetDlgItemText(IDC_SLEEP, CString("��"));
	SetDlgItemText(IDC_RATE, CString("�Զ�"));
}


HBRUSH CTCPͨ�ſյ��ն�MFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CString onoff;
	GetDlgItemText(IDC_ONOFF, onoff);
	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_ONOFF || pWnd->GetDlgCtrlID() == IDC_TEMPRATURE || pWnd->GetDlgCtrlID() == IDC_TIME
		|| pWnd->GetDlgCtrlID() == IDC_MODE || pWnd->GetDlgCtrlID() == IDC_SLEEP || pWnd->GetDlgCtrlID() == IDC_RATE)
	{
		if (onoff == "������")
		{
			pDC->SetTextColor(RGB(255, 0, 0));       
		}
		else if (onoff == "������")
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnOn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnRate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static int rate = 0;
	status.checkInstrucion("RATE", (rate++)%4);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnTime()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("TIME", 100);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnSleep2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("SLEEP", NULL);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnUp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("TEMPERATURE", 1);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnDown()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("TEMPERATURE", -1);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnMode()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static int mode = 0;
	status.checkInstrucion("MODE", (mode++)%3);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnClickedBtnOn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("POWER", NULL);
}


void CTCPͨ�ſյ��ն�MFCDlg::OnBnClickedBtnPower()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	status.checkInstrucion("POWER", NULL);
}


// TCPͨ��-�յ��ն�-MFCDlg.h : ͷ�ļ�
//

#pragma once


// CTCPͨ�ſյ��ն�MFCDlg �Ի���
class CTCPͨ�ſյ��ն�MFCDlg : public CDialogEx
{
// ����
public:
	CTCPͨ�ſյ��ն�MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCPMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	CWinThread *Thread1;
//	void Instructions(char *ins, int size);
	static UINT ThreadProc(LPVOID pParam);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnOn();
	afx_msg void OnBnClickedBtnRate();
	afx_msg void OnBnClickedBtnTime();
	afx_msg void OnBnClickedBtnSleep2();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedBtnMode();
	afx_msg void OnClickedBtnOn();
	afx_msg void OnBnClickedBtnPower();
};

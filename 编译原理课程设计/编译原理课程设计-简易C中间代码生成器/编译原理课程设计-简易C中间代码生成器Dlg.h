
// ����ԭ��γ����-����C�м����������Dlg.h : ͷ�ļ�
//

#pragma once


// C����ԭ��γ���Ƽ���C�м����������Dlg �Ի���
class C����ԭ��γ���Ƽ���C�м����������Dlg : public CDialogEx
{
// ����
public:
	C����ԭ��γ���Ƽ���C�м����������Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	CString midcodes, tmpcs;
// �Ի�������
	enum { IDD = IDD_C_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedExecute();
	afx_msg void OnBnClickedClear();
	afx_msg void OnStnClickedMiddlecodes();
	afx_msg void OnStnClickedCodes();
	void AppendText(int controlId, CString strAdd);
	double C����ԭ��γ���Ƽ���C�м����������Dlg::calculation();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::booleanstatement();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::ifstatement();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::statement();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::whilestatement();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::forstatement();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::declare();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::entrance();
	void C����ԭ��γ���Ƽ���C�м����������Dlg::compoundstatement();
	CFont font;
};


// TCPͨ��-�յ��ն�-MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTCPͨ�ſյ��ն�MFCApp: 
// �йش����ʵ�֣������ TCPͨ��-�յ��ն�-MFC.cpp
//

class CTCPͨ�ſյ��ն�MFCApp : public CWinApp
{
public:
	CTCPͨ�ſյ��ն�MFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTCPͨ�ſյ��ն�MFCApp theApp;

// ����ԭ��γ����-����C�м����������.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C����ԭ��γ���Ƽ���C�м����������App: 
// �йش����ʵ�֣������ ����ԭ��γ����-����C�м����������.cpp
//

class C����ԭ��γ���Ƽ���C�м����������App : public CWinApp
{
public:
	C����ԭ��γ���Ƽ���C�м����������App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C����ԭ��γ���Ƽ���C�м����������App theApp;
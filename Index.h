
// Index.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������
#define WM_MYMSG  WM_USER+10
#define WM_MYMSG1  WM_USER+11

// CIndexApp:
// � ���������� ������� ������ ��. Index.cpp
//

class CIndexApp : public CWinApp
{
public:
	CIndexApp();
	
// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CIndexApp theApp;
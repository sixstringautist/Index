
// Index.h : главный файл заголовка для приложения PROJECT_NAME
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#define WM_MYMSG  WM_USER+10
#define WM_MYMSG1  WM_USER+11

// CIndexApp:
// О реализации данного класса см. Index.cpp
//

class CIndexApp : public CWinApp
{
public:
	CIndexApp();
	
// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CIndexApp theApp;
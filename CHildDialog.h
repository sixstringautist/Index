#pragma once

// Диалоговое окно CHildDialog
#include "afxwin.h"
#include "afxcmn.h"
#include "MainDlg.h"
class CHildDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHildDialog)

public:
	CHildDialog(CWnd* pParent = nullptr); // стандартный конструктор
	virtual ~CHildDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	void ReadAndWrite();
	static CString path;
	std::map<CString, std::map<CString, std::vector<unsigned long>>> File;
	CProgressCtrl Progress;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	static UINT ThreadControl(LPVOID pParam); 
protected:
	afx_msg LRESULT OnMymsg(WPARAM wParam, LPARAM lParam);
public:
	static void SetPath(CString _path);
protected:
	afx_msg LRESULT OnMymsg1(WPARAM wParam, LPARAM lParam);
};

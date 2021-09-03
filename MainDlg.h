
// MainDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <fstream>
#include <list>
#include <map>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
// диалоговое окно CMainDlg
class CMainDlg : public CDialog
{
// Создание
public:
	CMainDlg(CWnd* pParent = NULL);	// стандартный конструктор
	static int Global;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INDEX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	


// Реализация
protected:
	HICON m_hIcon;
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonfind();
	afx_msg void OnBnClickedButtonshow();
	afx_msg void OnBnClickedAdddir();
	afx_msg void OnBnClickedButtontest();
	afx_msg void OnEnChangeInputpathlist();
	afx_msg void OnLbnDblclkListout();
	afx_msg void OnEnSetfocusFinder();
	afx_msg void OnBnClickeReindex();
	CButton Add;
	CListBox OutputControlList;
	CListBox Dir_s;
	CListBox IDir_s;
	CButton ButtonFind;
	CString InputWord;
	CString InputPath;
	CToolTipCtrl MyToolTip;
	enum class DataType { Word, Directory, Integer };
	static int GetFileList(std::vector<CString> & buf, CString _dir);
	static int GetFileList(CString _dir);
	static DataType WhatIsDataType(CString _token);
	static void SaveFileList(CString _path);
	CString ShowDir();
	void Find(CString _word);
	BOOL CMainDlg::PreTranslateMessage(MSG* pMsg);
	static void ReadFileIn(CString _path, std::map<CString, std::map<CString, std::vector<unsigned long>>> & _File);
	static void WriteIndex(std::map<CString, std::map<CString, std::vector<unsigned long>>> &_File);
	static void ReadFileList(CString _dir,std::vector<CString> &buf);
	static void ReadIndex(std::map<CString, std::map<CString, std::vector<unsigned long>>> &_File);
	static std::wstring ToUTF(std::string err);
	static void GetList(std::vector<CString> &list, CString name);
	static void WriteList(std::vector<CString> &list, CString name);
	static std::vector<CString>::const_iterator FindInVector(std::vector<CString> &list, CString obj);
	static CString FindInFile(CString _path, unsigned long offset);
	
	
};

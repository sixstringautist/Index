// CHildDialog.cpp: файл реализации
//

#include "stdafx.h"
#include "Index.h"
#include "CHildDialog.h"
#include "afxdialogex.h"

// Диалоговое окно CHildDialog

IMPLEMENT_DYNAMIC(CHildDialog, CDialogEx)
CHildDialog::CHildDialog(CWnd* pParent )
	: CDialogEx(IDD_CHILD, pParent)
{
	
}


CHildDialog::~CHildDialog()
{
}

void CHildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, Progress);
}


BEGIN_MESSAGE_MAP(CHildDialog, CDialogEx)
	
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_MYMSG, &CHildDialog::OnMymsg)
	ON_MESSAGE(WM_MYMSG1, &CHildDialog::OnMymsg1)
END_MESSAGE_MAP()


// Обработчики сообщений CHildDialog





void CHildDialog::ReadAndWrite()
{
	CWinThread * th = AfxBeginThread(ThreadControl, this);
}
UINT CHildDialog::ThreadControl(LPVOID pParam)
{
	std::vector<CString> CurrentList;
	std::vector<CString> RealList;
	std::vector<CString> NewFiles;
	std::vector<CString> DirectoryList;
	std::vector<CString> IndList;
	try
	{

		CHildDialog *di = static_cast<CHildDialog *>(pParam);
		CMainDlg::GetList(IndList, L"IndexedList.txt");
		CMainDlg::GetList(DirectoryList, L"DirectoryList.txt");
		CMainDlg::ReadFileList(CHildDialog::path.GetString(), CurrentList);
		CMainDlg::GetFileList(RealList, CHildDialog::path.GetString());

		auto iter = CMainDlg::FindInVector(IndList, CHildDialog::path.GetString());  //Повторная индексация если условие true
		if (iter != IndList.end()) 
		{
			//Если размеры векторов не равны, значит были изменения
			if (RealList.size() != CurrentList.size())
			{
				auto it = RealList.begin(); //Поиск новых файлов в папке, результат в NewFiles
				while (it != RealList.end())
				{
					auto it2 = CMainDlg::FindInVector(CurrentList, it->GetString());
					if (it2 != CurrentList.end())
					{
						it++;
					}
					else
					{
						NewFiles.push_back(it->GetString());
						it++;
					}
				}
				CMainDlg::ReadIndex(di->File);
				for (auto &el : NewFiles)
				{
					CMainDlg::ReadFileIn(el, di->File);
				}
				::PostMessage(di->GetSafeHwnd(), WM_MYMSG, 0, 0);
				CMainDlg::WriteIndex(di->File);
				CMainDlg::SaveFileList(CHildDialog::path.GetString());
				::PostMessage(di->GetSafeHwnd(), WM_MYMSG, 0, 0);
			}
			else { AfxMessageBox(L"Нет новых файлов в папке"); 
			::PostMessage(di->GetSafeHwnd(), WM_MYMSG1, 0, 0);}

		}
		else //Первая индексация
		{
			CMainDlg::ReadIndex(di->File);
			for (auto &el : CurrentList)
			{
				CMainDlg::ReadFileIn(el, di->File);
			}
			::PostMessage(di->GetSafeHwnd(), WM_MYMSG, 0, 0);
			CMainDlg::WriteIndex(di->File);
			std::wofstream out("IndexedList.txt", std::ios::app);
			out.imbue(std::locale("Russian_russia"));
			out << CHildDialog::path.GetString() << std::endl;
			out.close();
			auto it = CMainDlg::FindInVector(DirectoryList, CHildDialog::path);
			if (it != DirectoryList.end())
				DirectoryList.erase(it);
			CMainDlg::WriteList(DirectoryList, L"DirectoryList.txt");
			di->File.clear();
			::PostMessage(di->GetSafeHwnd(), WM_MYMSG, 0, 0);
		}
		
	}
	catch (const std::ios::failure &e)
	{
		std::wstring werr = CMainDlg::ToUTF(e.what());
		AfxMessageBox(werr.c_str());
	}
	catch (const std::exception & e)
	{

		std::wstring werr = CMainDlg::ToUTF(e.what());
		AfxMessageBox(werr.c_str());
	}

	return 0;
}


void CHildDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	Progress.SetRange(0, 100);
	ReadAndWrite();
}







afx_msg LRESULT CHildDialog::OnMymsg(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	Progress.SetPos(Progress.GetPos()+50);
	if (Progress.GetPos() == 100)
	{
		this->EndDialog(true);
	}
	return 0;
}


void CHildDialog::SetPath(CString _path)
{
	path = _path;
}


afx_msg LRESULT CHildDialog::OnMymsg1(WPARAM wParam, LPARAM lParam)
{
	this->EndDialog(true);
	return 0;
}

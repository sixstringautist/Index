
// MainDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Index.h"
#include "MainDlg.h"
#include "ChildDialog.h"
#include "afxdialogex.h"
#include <iostream>
#include <io.h>
#include "atlconv.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int CMainDlg::Global = 0;
// Диалоговое окно CAboutDlg используется для описания сведений о приложении
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// диалоговое окно CMainDlg



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_INDEX_DIALOG, pParent)
	, InputWord(_T(""))
	, InputPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINDER, InputWord);
	DDX_Text(pDX, IDC_INPUTPATHLIST, InputPath);
	DDX_Control(pDX, ID_ADDDIR, Add);
	DDX_Control(pDX, IDC_DIRECTORYLIST, Dir_s);
	DDX_Control(pDX, IDC_LISTOUT, OutputControlList);
	DDX_Control(pDX, IDC_LIST2, IDir_s);
	DDX_Control(pDX, IDC_BUTTONFIND, ButtonFind);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONFIND, &CMainDlg::OnBnClickedButtonfind)
	ON_BN_CLICKED(IDC_BUTTONSHOW, &CMainDlg::OnBnClickedButtonshow)
	ON_BN_CLICKED(ID_ADDDIR, &CMainDlg::OnBnClickedAdddir)
	ON_EN_CHANGE(IDC_INPUTPATHLIST, &CMainDlg::OnEnChangeInputpathlist)
	ON_BN_CLICKED(IDC_BUTTONTEST, &CMainDlg::OnBnClickedButtontest)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickeReindex)
	ON_LBN_DBLCLK(IDC_LISTOUT, &CMainDlg::OnLbnDblclkListout)
	ON_EN_SETFOCUS(IDC_FINDER, &CMainDlg::OnEnSetfocusFinder)
END_MESSAGE_MAP()


// обработчики сообщений CMainDlg

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	InputPath = L"Нажмите обзор что бы добавить новую директорию";
	InputWord = L"Введите слово для поиска";
	Add.EnableWindow(FALSE);
	UpdateData(FALSE);
	std::wifstream fin;
	try
	{

		//Create the ToolTip control
		if (!MyToolTip.Create(this))
		{
			TRACE0("Unable to create the ToolTip!");
		}
		else
		{
			// Add tool tips to the controls, either by hard coded string 
			// or using the string table resource
			MyToolTip.AddTool(&OutputControlList, L"Двойной щелчок для очистки");
			MyToolTip.Activate(TRUE);
		}




		std::vector<CString> list;
		GetList(list,L"DirectoryList.txt");
		for (auto &el:list)
		{
			Dir_s.InsertString(-1, el);
		}
		list.clear();
		GetList(list, L"IndexedList.txt");
		for (auto &el : list)
		{
			IDir_s.InsertString(-1, el);
		}
	}
	catch (const std::exception &ex)
	{
		std::wstring werr = ToUTF(ex.what());
		AfxMessageBox(werr.c_str());
	}
	setlocale(LC_CTYPE, "rus");

	

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainDlg::OnBnClickedButtonfind()
{
	UpdateData(TRUE);
	Find(InputWord.MakeLower());
}





CString CMainDlg::ShowDir()
{
	BROWSEINFO Browse;
	LPITEMIDLIST pidl;
	TCHAR Path[_MAX_DIR];
	CString CPath;
	ZeroMemory(&Browse, sizeof(Browse));
	Browse.hwndOwner = NULL;
	Browse.lpszTitle = L"Выберите папку";
	Browse.ulFlags = BIF_RETURNONLYFSDIRS;
	pidl = SHBrowseForFolder(&Browse);
	if (pidl)
	{
		SHGetPathFromIDList(pidl, Path);
		CPath = Path;
		return CPath;

	}
	else
		return NULL;
}


void CMainDlg::OnBnClickedButtonshow()
{
	InputPath = ShowDir();
	if (InputPath)
	{
		UpdateData(FALSE);
		Add.EnableWindow(TRUE);
	}
	else MessageBox(L"Ошибка", L"Не удалось открыть директорию!", MB_OK | MB_ICONERROR);
}


void CMainDlg::OnBnClickedAdddir()
{
	UpdateData(TRUE);
	using namespace std;
	wofstream fout;
	locale rus("Russian_russia");
	fout.imbue(rus);
	wifstream fin;
	fin.imbue(rus);
	CString path;
	try
	{
		fout.open("DirectoryList.txt", ios_base::app);
		fout << InputPath.GetString() << endl;
		Dir_s.InsertString(-1, InputPath);
		fout.close();
		SaveFileList(InputPath.GetString());
		InputPath = L"";
		Add.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	catch (const std::exception &ex)
	{
		wstring werr = ToUTF(ex.what());
		AfxMessageBox(werr.c_str());
	}
}


void CMainDlg::OnEnChangeInputpathlist()
{
	UpdateData(TRUE);
	if (InputPath.IsEmpty())
	{
		Add.EnableWindow(FALSE);
	}
	else Add.EnableWindow(TRUE);
}

int CMainDlg::GetFileList(std::vector<CString>  &buf, CString _dir)
{
		CString tmp;
		HANDLE hwd;
		WIN32_FIND_DATA Fdata;
		int k = 0;
		try
		{

			hwd = FindFirstFile(_dir + "\\*.txt", &Fdata);
			if (hwd != INVALID_HANDLE_VALUE)
			{			
					do
					{
						tmp = _dir + L"\\" + Fdata.cFileName;
						if (tmp.Compare(_dir + L"\\FileList.txt") == 0)
							continue;
						buf.push_back(tmp);
						k++;

					} while (FindNextFile(hwd, &Fdata) != 0);
				
			}
		}
		catch (const std::exception& ex)
		{
			std::wstring werr = ToUTF(ex.what());
			AfxMessageBox(werr.c_str());
			return NULL;
		}
		return k;

	
}








CMainDlg::DataType CMainDlg::WhatIsDataType(CString _token)
{
	int a;
	HANDLE hdl;
	WIN32_FIND_DATA test;
	hdl = FindFirstFile(_token, &test);
	if (hdl != INVALID_HANDLE_VALUE)
	{
		FindClose(hdl);
		return DataType::Directory;
	}
	else
	{
		for (int i = 0; i < _token.GetLength(); i++)
		{
			a = _token[i];
			if (a > 57 || a < 48)
			{
				return DataType::Word;
			}		
		}
		return DataType::Integer;
	}
}


void CMainDlg::SaveFileList(CString _path)
{
	using namespace std;
	vector<CString> buffer;
	GetFileList(buffer, _path);
	wofstream out;
	out.imbue(locale("Russian_russia"));
	try
	{
		out.open(_path + L"\\FileList.txt");
		for (int i = 0; i < buffer.size(); i++)
		{
			if (buffer[i].Compare(_path+L"\\FileList.txt") == 0)
				continue;
			out << buffer[i].GetString() << endl;
		}
		out.close();
	}
	catch (const std::exception &ex)
	{
		std::wstring werr = ToUTF(ex.what());
		AfxMessageBox(werr.c_str());
	}
	
	
	
}



CString CHildDialog::path = L"";
void CMainDlg::OnBnClickedButtontest()
{
	CString buf;
	std::vector<CString> IndList;
	Dir_s.GetText(Dir_s.GetCurSel(), buf);
	Dir_s.DeleteString(Dir_s.GetCurSel());
	CHildDialog MyDlg(this);
	CHildDialog::SetPath(buf);
	MyDlg.DoModal();
	GetList(IndList,L"IndexedList.txt");
	for (int i = 0; i < IDir_s.GetCount(); i++)
	{
		IDir_s.DeleteString(i);
	}
	for (auto &el : IndList)
		IDir_s.InsertString(-1, el);
}


void CMainDlg::Find(CString _word)
{
	using namespace std;
	map<CString, map<CString, vector<unsigned long>>> File;
	//for(int i=0;i<OutputControlList.GetCount();i++)
	//OutputControlList.DeleteString(i);
	ReadIndex(File);
	auto it = File.find(_word.GetString());
	if (it == File.end())
	{
		OutputControlList.InsertString(-1, L"Слово не найдено!");
	}
	else 
	{
		auto it2 = it->second.begin();
		while (it2!= it->second.end())
		{
			for (int i = 0; i < it2->second.size(); i++) 
			{
				OutputControlList.InsertString(-1, L"Файл: " + it2->first);
				OutputControlList.InsertString(-1, FindInFile(it2->first, it2->second.at(i)));
			}
			it2++;
		}
	}
}


void CMainDlg::ReadFileIn(CString _path, std::map<CString, std::map<CString, std::vector<unsigned long>>> & _File)
{
	using namespace std;
	wifstream in;
	in.imbue(locale("Russian_russia"));
	wstring buf;
	CString smb = L",!?-.\"\\'[]{}()|;_=0123456789«»";
	CString exc;
	CString cbuf;
	map<CString, vector<unsigned long>> tmp;
	wstringstream ss(ios::in|ios::out|ios::binary);
	ss.imbue(locale("Russian_russia"));
	ss.exceptions(istream::badbit);
	unsigned int tpos;
	in.open(_path,ios::binary);
	ss << in.rdbuf();
	in.close();
	
		while (ss >> buf)
		{
			cbuf = buf.c_str();
			exc = cbuf.SpanExcluding(smb);
			if (exc.GetLength() >= 2)
			{
				tpos = ss.tellg();
			if (tpos == -1 && ss.eof() == true)
			{
				ss.clear();
				ss.seekg(0, ios::end);
				tpos++;
				tpos -= exc.GetLength();
				auto it = _File.find(exc.MakeLower());
				if (it != _File.end())
				{
					auto it1 = it->second.find(_path);
					if (it1 != it->second.end())
					{
						it1->second.push_back(tpos);
					}
					else
						it->second.emplace(_path, vector<unsigned long>{tpos});
				}
				else
				{
					tmp.emplace(_path, vector<unsigned long> {tpos});
					_File.emplace(exc.MakeLower(), tmp);
					tmp.clear();
				}
			}
			else
			{
				tpos = ss.tellg();
				tpos -= exc.GetLength();
				auto it = _File.find(exc.MakeLower());
				if (it != _File.end())
				{
					auto it1 = it->second.find(_path);
					if (it1 != it->second.end())
					{
						it1->second.push_back(tpos);
					}
					else
						it->second.emplace(_path, vector<unsigned long>{tpos});
				}
				else
				{
					tmp.emplace(_path, vector<unsigned long> {tpos});
					_File.emplace(exc.MakeLower(), tmp);
					tmp.clear();
				}
			}
		}
		}
}


void CMainDlg::WriteIndex(std::map<CString, std::map<CString, std::vector<unsigned long>>> &_File)
{
	using namespace std;
		wofstream out;
		out.exceptions(ifstream::failbit | ifstream::badbit);
		out.imbue(locale("Russian_russia"));
		out.open(L"Index.txt");
		for (auto &it : _File)
		{
			out << it.first.GetString() << L";";
			for (auto &it2 : it.second)
			{
				out << it2.first.GetString() << L"|";
				for (int i = 0; i < it2.second.size();i++)
				{
					if (i == (it2.second.size() - 1))
					{
						out << it2.second.at(i)<< L";";
						break;
					}
					out <<it2.second.at(i) << L",";
				}
			}
			out << endl;

		}
		out.close();
	
}








int CMainDlg::GetFileList(CString _dir)
{
	int k = 0;
	HANDLE hwd;
	WIN32_FIND_DATA Fdata;
	hwd = FindFirstFile(_dir + "\\*.txt", &Fdata);
	if (hwd != INVALID_HANDLE_VALUE)
	{
		do
		{
			k++;

		} while (FindNextFile(hwd, &Fdata) != 0);
	}
	return k;
}


void CMainDlg::ReadFileList(CString _dir,std::vector<CString> &buf)
{
	using namespace std;
	_dir += L"\\FileList.txt";
	wifstream in;
	in.imbue(locale("Russian_russia"));
	in.open(_dir);
	CString pbuf;
	while (in.getline(pbuf.GetBufferSetLength(MAX_PATH), MAX_PATH))
	{
		buf.push_back(pbuf);
		pbuf.ReleaseBuffer();
	}
	in.close();
}


void CMainDlg::ReadIndex(std::map<CString, std::map<CString, std::vector<unsigned long>>> &_File)
{
	using namespace std;
	map<CString, vector<unsigned long>> tmp;
	wifstream in("Index.txt",ios::binary);
	in.imbue(locale("Russian_russia"));
	stringstream ss(ios::in | ios::out | ios::binary);
	wstring buf;
	CString Line;
	CString token;
	CString word;
	CString dir;
	int cur;
	while (getline(in, buf))
	{
		Line = buf.c_str();
		Line = Line.Trim();
		cur = 0;
		while ((token = Line.Tokenize(L";,|", cur)) != L"")
		{
			if (WhatIsDataType(token) == DataType::Word)
			{
				token.Remove(160); token.Remove(L' ');
			}
			switch (WhatIsDataType(token))
			{
			case DataType::Word:
				word = token;
				break;
			case DataType::Directory:
				dir = token;
				tmp.emplace(token, vector<unsigned long>{});
				break;
			case DataType::Integer:
				
				tmp[dir].push_back(_wtoi(token.GetString()));
				break;
			}
		}
		_File.emplace(word, tmp);
		tmp.clear();
	}
	in.close();
}


std::wstring CMainDlg::ToUTF(std::string err)
{
	wchar_t *buf = new wchar_t[err.length() + 1];
	MultiByteToWideChar(CP_ACP, 0, err.c_str(), -1, buf, err.length() + 1);
	std::wstring werr(buf);
	delete[] buf;
	return werr;
}


void CMainDlg::GetList(std::vector<CString> &list,CString name)
{
	using namespace std;
	CString buf;
	wifstream in(name);
	in.imbue(locale("Russian_russia"));
	while (in.getline(buf.GetBufferSetLength(MAX_PATH), MAX_PATH))
	{
		list.push_back(buf);
		buf.ReleaseBuffer();
	}
	in.close();
}


void CMainDlg::WriteList(std::vector<CString> &list, CString name)
{
	using namespace std;
	wofstream out("DirectoryList.txt");
	out.imbue(locale("Russian_russia"));
	for (auto &el : list)
	{
		out << el << endl;
	}
	out.close();
}


std::vector<CString>::const_iterator CMainDlg::FindInVector(std::vector<CString> &list,CString obj)
{
	auto it = list.begin();
	while (it!=list.end())
	{
		if (it->Compare(obj) == 0)
			break;
		it++;
	}
	return it;
}


void CMainDlg::OnBnClickeReindex()
{
	CString buf;
	std::vector<CString> IndList;
	IDir_s.GetText(IDir_s.GetCurSel(), buf);
	CHildDialog MyDlg(this);
	CHildDialog::SetPath(buf);
	MyDlg.DoModal();
}


CString CMainDlg::FindInFile(CString _path, unsigned long offset)
{
	using namespace std;
	CString buffer;
	try
	{
		wifstream input(_path);
		input.imbue(locale("Russian_russia"));
		input.seekg(0,ios::end);
		int length = input.tellg();
		input.seekg(ios::beg);
		
		if (offset < 60)
		{
			if (length < 60)
			{
				input.getline(buffer.GetBufferSetLength(length), length);
			}else
			input.getline(buffer.GetBufferSetLength(60), 60);
		}
		else 
		{
			input.seekg(offset - 30, ios::beg);
			input.read(buffer.GetBufferSetLength(60), 60);
		}
		
		input.close();
		return buffer;
	}
	catch (exception &e)
	{
		wstring err = ToUTF(e.what());
		AfxMessageBox(err.c_str());
	}
}


void CMainDlg::OnLbnDblclkListout()
{
	while (OutputControlList.DeleteString(0));
}


void CMainDlg::OnEnSetfocusFinder()
{
	InputWord = L"";
	UpdateData(FALSE);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	MyToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

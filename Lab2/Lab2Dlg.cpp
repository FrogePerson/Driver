
// Lab2Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Lab2.h"
#include "Lab2Dlg.h"
#include "afxdialogex.h"
#include "CSCM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CLab2Dlg



CLab2Dlg::CLab2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAB2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab2Dlg::print(LPCTSTR pstr)
{
	m_lst_log.InsertString(-1, pstr);
}

void CLab2Dlg::vprint(LPCTSTR pstr, va_list pargs)
{
	wchar_t tmp[1024];
	vswprintf(tmp, pstr, pargs);
	m_lst_log.InsertString(-1, tmp);
}

void CLab2Dlg::vprintErr(LPCTSTR pstr, va_list pargs)
{
	wchar_t tmp[1024];
	LPVOID lpMsgBuf;

	vswprintf(tmp, pstr, pargs);

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	wcscat(tmp, (wchar_t*)lpMsgBuf);

	LocalFree(lpMsgBuf);
	m_lst_log.InsertString(-1, tmp);
}

void CLab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_FNAME, m_edt_fname);
	DDX_Control(pDX, IDC_EDT_CNAME, m_edt_sname);
	DDX_Control(pDX, IDC_EDT_LNAME, m_edt_lname);
	DDX_Control(pDX, IDC_LIST1, m_lst_log);
}

BEGIN_MESSAGE_MAP(CLab2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FNAME, &CLab2Dlg::OnBnClickedBtnFname)
	ON_BN_CLICKED(IDC_BTN_ADD, &CLab2Dlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELL, &CLab2Dlg::OnBnClickedBtnDell)
	ON_BN_CLICKED(IDC_BTN_START, &CLab2Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CLab2Dlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CLab2Dlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CLab2Dlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// Обработчики сообщений CLab2Dlg

BOOL CLab2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CLab2Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CLab2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLab2Dlg::OnBnClickedBtnFname()
{
	CString str;

	CFileDialog cfd(TRUE);
	if (IDOK != cfd.DoModal())
		return;
	m_edt_fname.SetWindowText(cfd.GetPathName());
}


void CLab2Dlg::OnBnClickedBtnAdd()
{
	CSCM scm;
	CString sname, fname;
	m_edt_sname.GetWindowTextW(sname);
	m_edt_fname.GetWindowTextW(fname);
	scm.Add(sname, fname);

}


void CLab2Dlg::OnBnClickedBtnDell()
{
	CSCM scm;
	CString sname;
	m_edt_sname.GetWindowTextW(sname);
	scm.Del(sname);
}


void CLab2Dlg::OnBnClickedBtnStart()
{
	CSCM scm;
	CString sname;
	m_edt_sname.GetWindowTextW(sname);
	scm.Start(sname);
}


void CLab2Dlg::OnBnClickedBtnStop()
{
	CSCM scm;
	CString sname;
	m_edt_sname.GetWindowTextW(sname);
	scm.Stop(sname);
}


void CLab2Dlg::OnBnClickedBtnOpen()
{
	CSCM scm;
	CString sname;
	m_edt_sname.GetWindowTextW(sname);
	scm.Open(sname);
}


void CLab2Dlg::OnBnClickedBtnClose()
{
	CSCM scm;
	scm.Close();
}




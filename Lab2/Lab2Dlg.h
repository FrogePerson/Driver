
// Lab2Dlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CLab2Dlg
class CLab2Dlg : public CDialogEx
{
// Создание
public:
	CLab2Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAB2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFname();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDell();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	CEdit m_edt_fname;
	CEdit m_edt_sname;
	CEdit m_edt_lname;
	CListBox m_lst_log;
};

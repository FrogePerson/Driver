
// Lab2.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CLab2App:
// Сведения о реализации этого класса: Lab2.cpp
//

class CLab2App : public CWinApp
{
public:
	CLab2App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CLab2App theApp;

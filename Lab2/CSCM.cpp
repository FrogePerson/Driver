#include "pch.h"
#include "CSCM.h"
#include "Lab2.h"
#include "Lab2Dlg.h"

CSCM::CSCM()
{
	hscm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
}

CSCM::~CSCM()
{
	CloseServiceHandle(hscm);
}

void CSCM::Print(LPCTSTR pstr, ...)
{
	va_list pargs;
	va_start(pargs, pstr);
	((CLab2Dlg*)(theApp.m_pMainWnd))->vprint(pstr, pargs);
	va_end(pargs);
}

void CSCM::PrintErr(LPCTSTR pstr, ...)
{
	va_list pargs;
	va_start(pargs, pstr);
	((CLab2Dlg*)(theApp.m_pMainWnd))->vprintErr(pstr, pargs);
	va_end(pargs);
}

void CSCM::Add(LPCTSTR sname, LPCTSTR fname)
{
	SC_HANDLE hsrv = CreateService(hscm, sname, sname, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, fname, NULL, NULL, NULL, NULL, NULL);

	if (!hsrv)
	{
		CSCM::PrintErr(L"Cant add service\n");
		return;
	}
	CSCM::Print(L"Added service\n");
	CloseServiceHandle(hsrv);
	
}

void CSCM::Del(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		CSCM::PrintErr(L"Cant open service\n");
		return;
	}

	if (!DeleteService(hsrv))
	{
		CSCM::PrintErr(L"Cant delete service\n");
	}
	else
	{
		CSCM::Print(L"Service deleted\n");
	}
	CloseServiceHandle(hsrv);
}

void CSCM::Start(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		CSCM::PrintErr(L"Cant open service\n");
		return;
	}

	if (!StartService(hsrv, 0,	NULL))
	{
		CSCM::PrintErr(L"Cant start service\n");
	}
	else
	{
		CSCM::Print(L"Service started\n");
	}
	CloseServiceHandle(hsrv);
}

void CSCM::Stop(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		CSCM::PrintErr(L"Cant open service\n");
		return;
	}


	SERVICE_STATUS status;
	if (!ControlService(hsrv, SERVICE_CONTROL_STOP, &status))
	{
		CSCM::PrintErr(L"Cant conrtoll service\n");
	}
	else
	{
		if (status.dwCurrentState != SERVICE_STOP)
		{
			CSCM::PrintErr(L"Cant stop service\n");
		}
		else
		{
			CSCM::Print(L"Service stopted\n");
		}
	}
	CloseServiceHandle(hsrv);
}

HANDLE CSCM::hfile = INVALID_HANDLE_VALUE;
void CSCM::Open(LPCTSTR lname)
{
	if (INVALID_HANDLE_VALUE != hfile)
	{
		CloseHandle(hfile);
	}
	hfile = CreateFile(lname, FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if (hfile == INVALID_HANDLE_VALUE)
	{
		CSCM::PrintErr(L"INVALID_HANDLE_VALUE\n");
	}
	else
	{
		CSCM::Print(L"File opened\n");
	}
	
}

void CSCM::Close()
{
	CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
	CSCM::Print(L"File closed\n");
}



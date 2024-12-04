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

void CSCM::Add(LPCTSTR sname, LPCTSTR fname)
{
	SC_HANDLE hsrv = CreateService(hscm, sname, sname, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, fname, NULL, NULL, NULL, NULL, NULL);
	CSCM::Print(L"Added service\n");

	if (!hsrv)
	{
		return;
	}
	CloseServiceHandle(hsrv);
}

void CSCM::Del(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		return;
	}

	if (!DeleteService(hsrv))
	{

	}
	else
	{

	}
	CloseServiceHandle(hsrv);
}

void CSCM::Start(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		return;
	}

	if (!StartService(hsrv, 0,	NULL))
	{

	}
	else
	{

	}
	CloseServiceHandle(hsrv);
}

void CSCM::Stop(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);

	if (NULL == hsrv)
	{
		return;
	}


	SERVICE_STATUS status;
	if (!ControlService(hsrv, SERVICE_CONTROL_STOP, &status))
	{

	}
	else
	{
		if (status.dwCurrentState != SERVICE_STOP)
		{

		}
		else
		{

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

	}
	else
	{

	}
	
}

void CSCM::Close()
{
	CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
}



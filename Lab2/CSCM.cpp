#include "pch.h"
#include "CSCM.h"

CSCM::CSCM()
{
	hscm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
}

CSCM::~CSCM()
{
	CloseServiceHandle(hscm);
}

void CSCM::Add()
{
	SC_HANDLE hsrv = CreateService(hscm, );

	CloseServiceHandle(hsrv);
}

void CSCM::Del(LPCTSTR sname)
{
	SC_HANDLE hsrv = OpenService(hscm, sname, );

	CloseServiceHandle(hsrv);
}

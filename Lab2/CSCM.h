#pragma once
class CSCM
{
public:
	CSCM();
	~CSCM();

	void Print(LPCTSTR pstr, ...);
	void Add(LPCTSTR sname, LPCTSTR fname);
	void Del(LPCTSTR sname);
	void Start(LPCTSTR sname);
	void Stop(LPCTSTR sname);
	void Open(LPCTSTR lname);
	void Close();

private:
	SC_HANDLE hscm;
	static HANDLE hfile;
};


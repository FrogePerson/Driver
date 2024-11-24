#pragma once
class CSCM
{
public:
	CSCM();
	~CSCM();

	void Add();
	void Del(LPCTSTR sname);
	void Start();
	void Stop();
	void Open();
	void Close();

private:
	SC_HANDLE hscm;
};


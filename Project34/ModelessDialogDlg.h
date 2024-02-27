#pragma once
#include "header.h"

class CModelessDialog
{
public:
	CModelessDialog(void);
public:
	~CModelessDialog(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CModelessDialog* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	HWND hStatic, hListAll;
	static HWND hRegistrationDialog; 
	static HWND hAuthorizationDialog; 
	void SaveRegistrationData(const RegistrationData& data);
	void CheckAuthorization(HWND hwnd);

};
#pragma once
#include "header.h"
#include "ModelessDialogDlg.h"

class CMainModalDialog
{
public:
	CMainModalDialog(void);
public:
	~CMainModalDialog(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CMainModalDialog* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	HWND hListMain, hEditMain;
	static HWND hRegistrationDialog;
	static HWND hAuthorizationDialog; 
};
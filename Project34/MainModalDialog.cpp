#include "MainModalDialog.h"
#include "ModelessDialogDlg.h"

CMainModalDialog* CMainModalDialog::ptr = NULL;
HWND CMainModalDialog::hRegistrationDialog = NULL;
HWND CMainModalDialog::hAuthorizationDialog = NULL;

CMainModalDialog::CMainModalDialog(void)
{
	ptr = this;
	hRegistrationDialog = NULL;
	hAuthorizationDialog = NULL;
}

CMainModalDialog::~CMainModalDialog(void)
{

}

void CMainModalDialog::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL CMainModalDialog::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

	return TRUE;
}

void CMainModalDialog::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

	static CModelessDialog dlg;
	switch (id)
	{

	case IDC_BUTTON_REGISTRATION:
	{
		// Îòêðûòèå îêíà ðåãèñòðàöèè
		if (!hRegistrationDialog)
		{
			hRegistrationDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_REGISTRATION_FORM), hwnd, (DLGPROC)CModelessDialog::DlgProc);
			ShowWindow(hRegistrationDialog, SW_SHOWNORMAL);
		}
		hRegistrationDialog = NULL;

		return;
	}
	break;

	case IDC_BUTTON_AUTHORIZATION:
	{

		//ïåðåõîä ê îêíó àâòîðèçàöèè
		// Îòêðûòèå îêíà àâòîðèçàöèè
		if (!hAuthorizationDialog)
		{
			hAuthorizationDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_AUTHORIZATION), hwnd, (DLGPROC)CModelessDialog::DlgProc);
			ShowWindow(hAuthorizationDialog, SW_SHOWNORMAL);
		}

		hAuthorizationDialog = NULL;
		return;
	}
	break;
	}
}

BOOL CALLBACK CMainModalDialog::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
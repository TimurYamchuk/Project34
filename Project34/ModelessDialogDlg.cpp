�������, ��� ��� ��� � ���������� ����������� :

```cpp
#include "header.h"
#include "ModelessDialogDlg.h"

CModelessDialog* CModelessDialog::ptr = NULL;
HWND CModelessDialog::hRegistrationDialog = NULL; // �������������� ������� ������������ ����������
HWND CModelessDialog::hAuthorizationDialog = NULL; // ��������� ����������� ���� �����������
// ����������� ����� ��� ������������ ������ ����� ������
static bool hasErrorLogin = false;
static bool hasErrorPassword = false;
static bool hasErrorName = false;
static bool hasErrorAge = false;
// ���� ��� ������������ ������� ������ "�����������"
bool authButtonClicked = false;

CModelessDialog::CModelessDialog(void)
{
    ptr = this;
}

CModelessDialog::~CModelessDialog(void)
{

}

void CModelessDialog::Cls_OnClose(HWND hwnd)
{
    DestroyWindow(hwnd);
    hRegistrationDialog = NULL;
    hAuthorizationDialog = NULL;
}

BOOL CModelessDialog::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hRegistrationDialog = GetDlgItem(hwnd, IDD_DIALOG_REGISTRATION_FORM);
    hAuthorizationDialog = GetDlgItem(hwnd, IDD_DIALOG_AUTHORIZATION);
    return TRUE;
}

///////////////////

void CModelessDialog::SaveRegistrationData(const RegistrationData& data) {
    // ��������� ���� ��� ������ (� ������ ����������)
    FILE* file;
    errno_t err = _wfopen_s(&file, TEXT("registration_data.txt"), TEXT("a"));

    if (err == 0)
    {
        if (file)
        {
            // ���������� ������ � ����
            fwprintf(file, TEXT("Login: %s\nPassword: %s\nFull Name: %s\nAge: %d\n\n"),
                data.login, data.password, data.fullName, data.age);
            // ��������� ����
            fclose(file);
        }
        else
        {
            // ������: ���� �� ������� �������
            MessageBox(NULL, TEXT("Failed to open file for writing."), TEXT("Error"), MB_ICONERROR);
        }
    }
    else
    {
        // ������: �� ������� ������� ����
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, TEXT("Error opening file. Error code: %d"), err);
        MessageBox(NULL, errorMsg, TEXT("Error"), MB_ICONERROR);
    }
}

void CModelessDialog::CheckAuthorization(HWND hwnd)
{
    // �������� ����������� ����� ����� ������ � ������
    HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_AUTH_LOGIN);
    HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_AUTH_PASSWORD);

    // �������� ��������� ������������� �������� ������ � ������
    wchar_t login[256];
    wchar_t password[256];
    GetWindowText(hEditLogin, login, sizeof(login) / sizeof(login[0]));
    GetWindowText(hEditPassword, password, sizeof(password) / sizeof(password[0]));

    bool logOk = false;
    bool pasOk = false;

    // ��������� ���� � ���������������� ������� ��� ������
    FILE* file;
    errno_t err = _wfopen_s(&file, TEXT("registration_data.txt"), TEXT("r"));

    if (err == 0 && file)
    {
        wchar_t buffer[256];
        // ������ ���� ��������� � ���������� ��������� �������� � ������� �� �����
        while (fgetws(buffer, sizeof(buffer), file))
        {
            wchar_t storedLogin[256];
            wchar_t storedPassword[256];

            if (swscanf_s(buffer, TEXT("Login: %255s"), storedLogin, sizeof(storedLogin) / sizeof(storedLogin[0])) == 1)
            {
                if (wcslen(storedLogin) > 0)
                {
                    if (wcscmp(login, storedLogin) == 0)
                    {
                        logOk = true;
                    }
                }
            }

            if (swscanf_s(buffer, TEXT("Password: %255s"), storedPassword, sizeof(storedPassword) / sizeof(storedPassword[0])) == 1)
            {
                if (wcslen(storedPassword) > 0)
                {
                    if (wcscmp(password, storedPassword) == 0)
                    {
                        pasOk = true;
                    }
                }
            }
        }

        if (pasOk && logOk)
        {
            // ���� ������ � ����� �������, ������� ��������� �� �������� �����������
            MessageBox(hwnd, TEXT("�� ������� ������������!"), TEXT("�����"), MB_ICONINFORMATION);
            fclose(file);
            return;
        }

        // ���� ������ ������� �������, ������� ��������� �� ������
        if (!authButtonClicked && wcslen(login) > 0 && wcslen(password) > 0)
        {
            MessageBox(hwnd, TEXT("�������� ����� ��� ������"), TEXT("������"), MB_ICONERROR);
        }

        fclose(file);
    }
    else
    {
        // ���� ���� � ������� �� ������� �������, ������� ��������� �� ������
        if (!authButtonClicked)
        {
            MessageBox(hwnd, TEXT("������ �������� ����� � ���������������� �������"), TEXT("������"), MB_ICONERROR);
        }
    }
}

///////////////////

void CModelessDialog::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
        // ��������� ������� ��� ����� ����� ��������������� ������
    case IDC_EDIT_REG_LOG:
    {
        switch (codeNotify)
        {
        case EN_KILLFOCUS:
        {
            HWND hEditLog = GetDlgItem(hwnd, IDC_EDIT_REG_LOG);
            wchar_t buffer[256];
            GetWindowText(hEditLog, buffer, sizeof(buffer) / sizeof(buffer[0]));
            int len = wcslen(buffer);
            hasErrorLogin = false;

            if (len < 4 || len > 20)
            {
                hasErrorLogin = true;
            }

            break;
        }
        }
        return;
    }
    break;

    case IDC_EDIT_REG_PASS:
    {
        switch (codeNotify)
        {
        case EN_KILLFOCUS:
        {
            HWND hEditPass = GetDlgItem(hwnd, IDC_EDIT_REG_PASS);
            wchar_t buffer[256];
            GetWindowText(hEditPass, buffer, sizeof(buffer) / sizeof(buffer[0]));
            int len = wcslen(buffer);
            bool hasDigit = false;

            for (int i = 0; i < len; ++i)
            {
                if (isdigit(buffer[i]))
                {
                    hasDigit = true;
                    break;
                }
            }



            hasErrorPassword = false;

            if (len < 4 || len > 20 || !hasDigit)
            {
                hasErrorPassword = true;
            }

            break;
        }
        }
        return;
    }
    break;

    case IDC_EDIT_REG_NAM:
    {
        switch (codeNotify)
        {
        case EN_KILLFOCUS:
        {
            HWND hEditName = GetDlgItem(hwnd, IDC_EDIT_REG_NAM);
            wchar_t buffer[256];
            GetWindowText(hEditName, buffer, sizeof(buffer) / sizeof(buffer[0]));
            int len = wcslen(buffer);
            hasErrorName = false;

            if (len < 4 || len > 40)
            {
                hasErrorName = true;
            }

            break;
        }
        }
        return;
    }
    break;

    case IDC_EDIT_REG_AGE:
    {
        switch (codeNotify)
        {
        case EN_KILLFOCUS:
        {
            HWND hEditAge = GetDlgItem(hwnd, IDC_EDIT_REG_AGE);
            wchar_t buffer[256];
            GetWindowText(hEditAge, buffer, sizeof(buffer) / sizeof(buffer[0]));
            int age = _wtoi(buffer);
            hasErrorAge = false;

            if (age < 1 || age > 140)
            {
                hasErrorAge = true;
            }

            break;
        }
        }
        return;
    }
    break;

    case IDC_BUTTON_REG_SAVE:
    {
        if (!hasErrorLogin && !hasErrorPassword && !hasErrorName && !hasErrorAge)
        {
            RegistrationData data;
            GetDlgItemText(hwnd, IDC_EDIT_REG_LOG, data.login, sizeof(data.login) / sizeof(data.login[0]));
            GetDlgItemText(hwnd, IDC_EDIT_REG_PASS, data.password, sizeof(data.password) / sizeof(data.password[0]));
            GetDlgItemText(hwnd, IDC_EDIT_REG_NAM, data.fullName, sizeof(data.fullName) / sizeof(data.fullName[0]));
            WCHAR buffer[256];
            GetDlgItemText(hwnd, IDC_EDIT_REG_AGE, buffer, sizeof(buffer) / sizeof(buffer[0]));
            data.age = _wtoi(buffer);

            SaveRegistrationData(data);

            SetDlgItemText(hwnd, IDC_EDIT_REG_LOG, TEXT(""));
            SetDlgItemText(hwnd, IDC_EDIT_REG_PASS, TEXT(""));
            SetDlgItemText(hwnd, IDC_EDIT_REG_NAM, TEXT(""));
            SetDlgItemText(hwnd, IDC_EDIT_REG_AGE, TEXT(""));

            MessageBox(hwnd, TEXT("����������� ���������. ����������, �������������."), TEXT("�����"), MB_ICONINFORMATION);
        }
        else
        {
            if (hasErrorLogin)
            {
                MessageBox(hwnd, TEXT("����� ������ ��������� �� 4 �� 20 ��������"), TEXT("������"), MB_ICONERROR);
            }

            if (hasErrorPassword)
            {
                MessageBox(hwnd, TEXT("������ ������ ��������� �� 4 �� 20 �������� � ��������� ���� �� ���� �����"), TEXT("������"), MB_ICONERROR);
            }

            if (hasErrorName)
            {
                MessageBox(hwnd, TEXT("��� ������ ��������� �� 4 �� 40 ��������"), TEXT("������"), MB_ICONERROR);
            }

            if (hasErrorAge)
            {
                MessageBox(hwnd, TEXT("������������ �������. ������� �������� �� 1 �� 140"), TEXT("������"), MB_ICONERROR);
            }
        }
    }
    break;

    case IDCANCEL:
    {
        Cls_OnClose(hwnd);
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }

    case ID_OK_AUTH:
    {
        CheckAuthorization(hwnd);
    }
    break;
    }
}

BOOL CALLBACK CModelessDialog::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}
```
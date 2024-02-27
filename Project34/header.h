#pragma once
#include <windows.h>
#include <windowsX.h>
#include <tchar.h>
#include "resource.h"
#include <cstdio>
#include <Windows.h>


struct RegistrationData {
    TCHAR login[256];
    TCHAR password[256];
    TCHAR fullName[256];
    int age;
};
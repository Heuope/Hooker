#include <cstdio>
#include <windows.h>
#include <detours.h>

TCHAR dllName[] = TEXT("HookerDll");

int main(int argc, wchar_t* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    si.cb = sizeof(STARTUPINFO);

    char* DirPath = new char[MAX_PATH];
    char* DLLPath = new char[MAX_PATH]; //testdll.dll
    char* DetourPath = new char[MAX_PATH]; //detoured.dll

    GetCurrentDirectory(MAX_PATH, (LPWSTR)DirPath);

    HINSTANCE hMyDll;
    if ((hMyDll = LoadLibrary(dllName)) == NULL)
    {
        return 1;
    }

    DetourCreateProcessWithDll(L"C:\\windows\\notepad.exe", NULL, NULL, NULL, false, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi, (LPCSTR)dllName, NULL);

    delete[] DirPath;
    delete[] DLLPath;
    delete[] DetourPath;
    return 0;
}
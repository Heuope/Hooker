#include <cstdio>
#include <windows.h>
#include <detours.h>

int main(int argc, wchar_t* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    si.cb = sizeof(STARTUPINFO);

    DetourCreateProcessWithDll(L"..\\Hooker\\Debug\\Test.exe", NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi, "..\\Hooker\\Debug\\HookerDll.dll", NULL);
    ResumeThread(pi.hThread);

    return 0;
}
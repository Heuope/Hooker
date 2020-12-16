#include <windows.h>
#include <detours.h>
#include <iostream>
#include <fstream>
#include <string>

void LogMessage(std::string message)
{
	std::ofstream out("Logs.txt", std::ios::app);
	if (out.is_open())
	{
		out << message << std::endl;
	}
	out.close();
}

HANDLE(WINAPI* RealCreateFile)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFile;

HANDLE WINAPI HookCreateFile(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	LogMessage("Program call CreateFile");

	return RealCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HFILE(WINAPI* RealOpenFile)(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle) = OpenFile;

HFILE WINAPI HookOpenFile(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle)
{
	LogMessage("Program call WriteFile");

	return RealOpenFile(lpFileName, lpReOpenBuff, uStyle);
}

BOOL(WINAPI* RealReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = ReadFile;

BOOL WINAPI HookReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	LogMessage("Program call ReadFile");

	return RealReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL(WINAPI* RealWriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = WriteFile;

BOOL WINAPI HookWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	LogMessage("Program call WriteFile");

	return RealWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL (WINAPI* RealMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) = MessageBox;

BOOL HookMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) 
{
	LogMessage("Programm call MessageBox");

	return RealMessageBox(hWnd, lpText, lpCaption, uType);
}

LSTATUS(WINAPI* RealRegGetValue)(HKEY hkey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData) = RegGetValueA;

LSTATUS WINAPI HookRegGetValue(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	LogMessage("Program call RegGetValue");

	return RealRegGetValue(hKey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}

LSTATUS(WINAPI* RealRegSetValue)(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData) = RegSetValueA;

LSTATUS WINAPI HookRegSetValue(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
	LogMessage("Program call RegSetValue");

	return RealRegSetValue(hKey, lpSubKey, dwType, lpData, cbData);
}

LSTATUS(WINAPI* RealRegOpenKey)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult) = RegOpenKey;

LSTATUS WINAPI HookRegOpenKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	LogMessage("Program call RegOpenKey");

	return RealRegOpenKey(hKey, lpSubKey, phkResult);
}

LSTATUS(WINAPI* RealRegCloseKey)(HKEY hKey) = RegCloseKey;

LSTATUS WINAPI HookRegCloseKey(HKEY hKey)
{
	LogMessage("Program call RegCloseKey");

	return RealRegCloseKey(hKey);
}

BOOL APIENTRY Source(HINSTANCE hInstance, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
		case DLL_PROCESS_ATTACH:
			DetourRestoreAfterWith();
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)RealCreateFile, HookCreateFile);
			DetourAttach(&(PVOID&)RealReadFile, HookReadFile);
			DetourAttach(&(PVOID&)RealWriteFile, HookWriteFile);
			DetourAttach(&(PVOID&)RealOpenFile, HookOpenFile);
			DetourAttach(&(PVOID&)RealMessageBox, HookMessageBox);
			DetourAttach(&(PVOID&)RealRegGetValue, HookRegGetValue);
			DetourAttach(&(PVOID&)RealRegSetValue, HookRegSetValue);
			DetourAttach(&(PVOID&)RealRegOpenKey, HookRegOpenKey);
			DetourAttach(&(PVOID&)RealRegCloseKey, HookRegCloseKey);
			DetourTransactionCommit();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			DetourRestoreAfterWith();
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)RealCreateFile, HookCreateFile);
			DetourDetach(&(PVOID&)RealReadFile, HookReadFile);
			DetourDetach(&(PVOID&)RealWriteFile, HookWriteFile);
			DetourDetach(&(PVOID&)RealOpenFile, HookOpenFile);
			DetourDetach(&(PVOID&)RealMessageBox, HookMessageBox);
			DetourDetach(&(PVOID&)RealRegGetValue, HookRegGetValue);
			DetourDetach(&(PVOID&)RealRegSetValue, HookRegSetValue);
			DetourDetach(&(PVOID&)RealRegOpenKey, HookRegOpenKey);
			DetourDetach(&(PVOID&)RealRegCloseKey, HookRegCloseKey);
			DetourTransactionCommit();
			break;
	}
	return FALSE;
}

extern "C" _declspec(dllexport) void Hook()
{
	
}
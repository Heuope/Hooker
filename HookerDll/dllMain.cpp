#include <windows.h>
#include <detours.h>
#include <iostream>
#include <fstream>
#include <string>

std::string getPathName(const std::string& s) 
{
	char sep = '\\';

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) 
	{
		return(s.substr(0, i));
	}

	return("");
}

std::ofstream out;

HANDLE(WINAPI* RealCreateFile)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFile;
HFILE(WINAPI* RealOpenFile)(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle) = OpenFile;
BOOL(WINAPI* RealReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = ReadFile;
BOOL(WINAPI* RealWriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = WriteFile;
BOOL(WINAPI* RealMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) = MessageBox;
LSTATUS(WINAPI* RealRegGetValue)(HKEY hkey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData) = RegGetValueA;
LSTATUS(WINAPI* RealRegSetValue)(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData) = RegSetValueA;
LSTATUS(WINAPI* RealRegOpenKey)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult) = RegOpenKey;
LSTATUS(WINAPI* RealRegCloseKey)(HKEY hKey) = RegCloseKey;

HANDLE WINAPI HookCreateFile(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (out.is_open())
	{
		out << "Program call CreateFile" << std::endl;
	}

	return RealCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HFILE WINAPI HookOpenFile(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle)
{
	if (out.is_open())
	{
		out << "Program call OpenFile" << std::endl;
	}
	return RealOpenFile(lpFileName, lpReOpenBuff, uStyle);
}

BOOL WINAPI HookReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if (out.is_open())
	{
		out << "Program call ReadFile" << std::endl;
	}

	return RealReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL WINAPI HookWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if (out.is_open())
	{
		out << "Program call WriteFile" << std::endl;
	}

	return RealWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL HookMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) 
{
	if (out.is_open())
	{
		out << "Programm call MessageBox" << std::endl;
	}

	return RealMessageBox(hWnd, lpText, lpCaption, uType);
}

LSTATUS WINAPI HookRegGetValue(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	if (out.is_open())
	{
		out << "Program call RegGetValue" << std::endl;
	}

	return RealRegGetValue(hKey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}

LSTATUS WINAPI HookRegSetValue(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
	if (out.is_open())
	{
		out << "Program call RegSetValue" << std::endl;
	}

	return RealRegSetValue(hKey, lpSubKey, dwType, lpData, cbData);
}

LSTATUS WINAPI HookRegOpenKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	if (out.is_open())
	{
		out << "Program call RegOpenKey" << std::endl;
	}

	return RealRegOpenKey(hKey, lpSubKey, phkResult);
}

LSTATUS WINAPI HookRegCloseKey(HKEY hKey)
{
	if (out.is_open())
	{
		out << "Program call RegCloseKey" << std::endl;
	}

	return RealRegCloseKey(hKey);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
		case DLL_PROCESS_ATTACH:

			char str[255];
			GetModuleFileNameA(hModule, str, 255);
			out.open((std::string(getPathName(std::string(str))) + std::string("\\log.txt")).c_str());

			DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			DetourAttach(&(PVOID&)RealCreateFile, HookCreateFile);
			DetourAttach(&(PVOID&)RealReadFile, HookReadFile);
			//DetourAttach(&(PVOID&)RealWriteFile, HookWriteFile);
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
			break;
	}
	return TRUE;
}
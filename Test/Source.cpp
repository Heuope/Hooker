//#include <windows.h>
//
//#define MAX_TRIAL_RUNS 5
//
//const TCHAR szCounterFileName[] = L"counter.dat";
//const TCHAR szMsgTmpl[] = L"Вы запустили программу в %d-й раз. %s.";
//const TCHAR szCheckOk[] = L"Все в порядке, продолжайте работу";
//const TCHAR szCheckFailed[] = L"Триал истек, купите полную версию";
//
//DWORD ReadCounter() 
//{
//    DWORD dwCounter, dwTemp;
//    HANDLE hFile = CreateFile(szCounterFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (INVALID_HANDLE_VALUE == hFile) 
//    {
//        return 1;
//    }
//
//    ReadFile(hFile, &dwCounter, sizeof(dwCounter), &dwTemp, NULL);
//
//    if (sizeof(dwCounter) != dwTemp) 
//    {
//        CloseHandle(hFile);
//        return 1;
//    }
//    CloseHandle(hFile);
//    return dwCounter;
//}
//
//VOID WriteCounter(DWORD dwCounter) 
//{
//    DWORD dwTemp;
//    HANDLE hFile = CreateFile(szCounterFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (INVALID_HANDLE_VALUE == hFile) 
//    {
//        return;
//    }
//    WriteFile(hFile, &dwCounter, sizeof(dwCounter), &dwTemp, NULL);
//    CloseHandle(hFile);
//}
//
//int main() 
//{
//    TCHAR szMsg[256];
//    DWORD dwCounter = ReadCounter();
//    LPCWSTR lpCheckResult = dwCounter > MAX_TRIAL_RUNS ? szCheckFailed : szCheckOk;
//    wsprintf(szMsg, szMsgTmpl, dwCounter, lpCheckResult);
//    MessageBox(0, szMsg, L"Сообщение", 0);
//
//    if (dwCounter <= MAX_TRIAL_RUNS) 
//    {
//        WriteCounter(dwCounter + 1);
//    }
//
//    ExitProcess(0);
//}

#include <iostream>
#include <windows.h>
//#include <memoryapi.h>
#include <string>

TCHAR keyName[] = TEXT("WinApiCriticalSection");

int main()
{
	HANDLE fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, keyName);

	if (fileMapping == NULL)
	{
		return -1;
	}

	auto cs = (CRITICAL_SECTION*)MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));

	if (cs == NULL)
	{
		CloseHandle(fileMapping);
		return -2;
	}

	int processId = GetCurrentProcessId();

	while (!TryEnterCriticalSection(cs))
	{
		std::cout << "Process " << processId << " wait" << std::endl;
		Sleep(10);
	}

	std::cout << "Process " << processId << " entered" << std::endl;
	std::cout << "Process " << processId << " working hard" << std::endl;
	std::cout << "Process " << processId << " leave" << std::endl;

	LeaveCriticalSection(cs);

	UnmapViewOfFile(cs);
	CloseHandle(fileMapping);

	return 0;
}
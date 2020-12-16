#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
    HANDLE hFile;
    char DataBuffer[] = "TEST";
    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    hFile = CreateFile(L"", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    OFSTRUCT buffer;
    HFILE hfile = OpenFile("hello.txt", &buffer, OF_READ);
    char buffer_read[6];
    DWORD bytes_read = 0;
    return 0;
}
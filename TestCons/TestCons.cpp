#include <Windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    HANDLE h = CreateFile(L"\\\\.\\Lab1Link\\qqq\\www.txt", FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == h)
    {
        printf("ERR: CreateFile errcode=%d\n", GetLastError());
        return -1;
    }

    printf("OK: CreateFile, press any key\n");
    _getche();
    CloseHandle(h);
    printf("press any key to close\n");
    _getche();

}


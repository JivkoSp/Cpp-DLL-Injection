#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>

int main()
{
    HMODULE hDll = LoadLibraryW(_T("x1ct"));

    if (!hDll || hDll == INVALID_HANDLE_VALUE)
    {
        std::cout << "Invalid handle!\n";
        return 1;
    }

    return 0;
}
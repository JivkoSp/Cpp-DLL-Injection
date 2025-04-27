#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>

typedef int(__stdcall* CallAx12)();

int main()
{
    HMODULE hDll = LoadLibraryW(_T("x1ct"));
   
    if (!hDll || hDll == INVALID_HANDLE_VALUE)
    {
        std::cout << "Invalid handle!\n";
        return 1;
    }

    CallAx12 callAx12 = (CallAx12)GetProcAddress(hDll, "CallAx12");

    if (!callAx12)
    {
        std::cout << "Invalid function call!\n";
        return 1;
    }

    callAx12();

    return 0;
}
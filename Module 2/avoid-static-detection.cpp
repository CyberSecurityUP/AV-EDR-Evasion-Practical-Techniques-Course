// Filename: avoid_static_detection.cpp
// Purpose: Avoid static signatures by using obfuscated strings and runtime resolution.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

int main() {
    char mbox[] = { 'M','e','s','s','a','g','e','B','o','x','A','\0' };
    HMODULE user32 = LoadLibraryA("user32.dll");
    FARPROC msgFunc = GetProcAddress(user32, mbox);
    
    typedef int (WINAPI* MSGBOX)(HWND, LPCSTR, LPCSTR, UINT);
    MSGBOX msg = (MSGBOX)msgFunc;

    msg(NULL, "Dynamic API call. Harder to detect statically.", "Evasion", MB_OK);
    return 0;
}

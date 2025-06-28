// Filename: subsystem_check.cpp
// Purpose: Identify the subsystem type and demonstrate interaction with Windows APIs.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

int main() {
    DWORD type = GetVersion();
    DWORD major = (DWORD)(LOBYTE(LOWORD(type)));
    DWORD minor = (DWORD)(HIBYTE(LOWORD(type)));

    std::cout << "Running on Windows Version: " << major << "." << minor << std::endl;

    MessageBoxA(NULL, "Interfacing with Win32 subsystem", "Subsystem", MB_OK);
    return 0;
}

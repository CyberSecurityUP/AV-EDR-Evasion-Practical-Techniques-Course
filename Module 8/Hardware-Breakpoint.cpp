// Filename: hardware_breakpoint_evasion.cpp
// Description: Set a hardware breakpoint to detect inline hooks and evade user-mode API hooks
// Author: Joas A Santos

#include <windows.h>
#include <iostream>

int SetHardwareBreakpoint() {
    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    HANDLE thread = GetCurrentThread();
    if (!GetThreadContext(thread, &ctx)) return 1;

    ctx.Dr0 = (DWORD_PTR)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
    ctx.Dr7 = 0x00000001; // Enable local breakpoint on DR0

    if (!SetThreadContext(thread, &ctx)) return 1;

    std::cout << "[+] Hardware breakpoint set on NtOpenProcess\n";
    return 0;
}

int main() {
    SetHardwareBreakpoint();
    // Use NtOpenProcess dynamically here to test hook evasion
    return 0;
}

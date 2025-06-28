// Filename: syscall_bypass.cpp
// Purpose: Bypass userland API hooks by jumping directly to syscalls in ntdll.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

extern "C" NTSTATUS NtQueryInformationProcess(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);

int main() {
    PROCESS_BASIC_INFORMATION pbi;
    ULONG len = 0;

    NTSTATUS status = NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), &len);

    if (status == 0) {
        std::cout << "NtQueryInformationProcess succeeded." << std::endl;
    } else {
        std::cout << "Syscall failed. Status: 0x" << std::hex << status << std::endl;
    }

    return 0;
}

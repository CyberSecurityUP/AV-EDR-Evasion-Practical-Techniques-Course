// Filename: ntapi_stager.cpp
// Purpose: Avoid Win32 API and use only NTAPI (NtWriteVirtualMemory, NtCreateThreadEx).
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <winternl.h>
#include <iostream>

typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(
    HANDLE, PVOID, PVOID, ULONG, PULONG
);

int main() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    pNtWriteVirtualMemory NtWriteVirtualMemory = (pNtWriteVirtualMemory)GetProcAddress(ntdll, "NtWriteVirtualMemory");

    HANDLE hProc = GetCurrentProcess();
    char buffer[] = "Hello from NtWrite!";
    PVOID remote = VirtualAlloc(NULL, sizeof(buffer), MEM_COMMIT, PAGE_READWRITE);

    NTSTATUS status = NtWriteVirtualMemory(hProc, remote, buffer, sizeof(buffer), NULL);
    std::cout << "NtWriteVirtualMemory status: 0x" << std::hex << status << std::endl;

    VirtualFree(remote, 0, MEM_RELEASE);
    return 0;
}

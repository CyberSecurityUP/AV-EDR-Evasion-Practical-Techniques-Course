// Filename: peb_ldr_walk.cpp
// Purpose: Access PEB directly and list loaded modules for stealth API resolution.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <winternl.h>
#include <iostream>

#pragma comment(lib, "ntdll.lib")

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    PVOID Reserved1[2];
    PVOID DllBase;
    UNICODE_STRING FullDllName;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
} PEB, *PPEB;

#ifdef _M_X64
#define PPEB_CURRENT ((PPEB)__readgsqword(0x60))
#else
#define PPEB_CURRENT ((PPEB)__readfsdword(0x30))
#endif

int main() {
    PPEB peb = PPEB_CURRENT;
    PLIST_ENTRY head = &peb->Ldr->InLoadOrderModuleList;
    PLIST_ENTRY curr = head->Flink;

    while (curr != head) {
        PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(curr, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        wprintf(L"Loaded Module: %wZ\n", &entry->FullDllName);
        curr = curr->Flink;
    }

    return 0;
}

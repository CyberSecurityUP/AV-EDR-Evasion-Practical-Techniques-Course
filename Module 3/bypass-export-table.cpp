// Filename: bypass_export_table.cpp
// Purpose: Bypass hooks by parsing DLL export table manually instead of using GetProcAddress.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

FARPROC GetExportByName(HMODULE mod, const char* funcName) {
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)mod;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)mod + dos->e_lfanew);
    DWORD expRVA = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)mod + expRVA);

    DWORD* names = (DWORD*)((BYTE*)mod + exp->AddressOfNames);
    WORD* ordinals = (WORD*)((BYTE*)mod + exp->AddressOfNameOrdinals);
    DWORD* funcs = (DWORD*)((BYTE*)mod + exp->AddressOfFunctions);

    for (DWORD i = 0; i < exp->NumberOfNames; ++i) {
        char* name = (char*)mod + names[i];
        if (strcmp(name, funcName) == 0) {
            return (FARPROC)((BYTE*)mod + funcs[ordinals[i]]);
        }
    }

    return NULL;
}

int main() {
    HMODULE kernel = GetModuleHandleA("kernel32.dll");
    FARPROC exitFn = GetExportByName(kernel, "ExitProcess");

    typedef void(WINAPI* ExitType)(UINT);
    ((ExitType)exitFn)(0);
    return 0;
}

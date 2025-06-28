// Filename: iat_hook.cpp
// Purpose: Hook a function via IAT and optionally hide the hook afterward.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

typedef int (WINAPI* pMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
pMessageBoxA originalMessageBoxA;

int WINAPI hookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    return originalMessageBoxA(hWnd, "Hooked!", lpCaption, uType);
}

void HookIAT() {
    HMODULE mod = GetModuleHandle(NULL);
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)mod;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)mod + dos->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR import = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)mod +
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    while (import->Name) {
        char* dllName = (char*)mod + import->Name;
        if (_stricmp(dllName, "user32.dll") == 0) {
            PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((BYTE*)mod + import->FirstThunk);
            while (thunk->u1.Function) {
                FARPROC* func = (FARPROC*)&thunk->u1.Function;
                if (*func == (FARPROC)MessageBoxA) {
                    DWORD old;
                    VirtualProtect(func, sizeof(FARPROC), PAGE_EXECUTE_READWRITE, &old);
                    originalMessageBoxA = (pMessageBoxA)*func;
                    *func = (FARPROC)hookedMessageBoxA;
                    VirtualProtect(func, sizeof(FARPROC), old, &old);
                    return;
                }
                ++thunk;
            }
        }
        ++import;
    }
}

int main() {
    HookIAT();
    MessageBoxA(NULL, "Test", "IAT Hook", MB_OK);
    return 0;
}

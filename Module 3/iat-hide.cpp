// Filename: iat_hide.cpp
// Purpose: Hide IAT entries by erasing them or replacing with junk pointers.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

void zero_iat() {
    HMODULE mod = GetModuleHandle(NULL);
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)mod;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)mod + dos->e_lfanew);

    PIMAGE_IMPORT_DESCRIPTOR import = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)mod +
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    while (import->Name) {
        PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((BYTE*)mod + import->FirstThunk);
        while (thunk->u1.Function) {
            DWORD old;
            VirtualProtect(&thunk->u1.Function, sizeof(LPVOID), PAGE_EXECUTE_READWRITE, &old);
            thunk->u1.Function = 0; // clear pointer
            VirtualProtect(&thunk->u1.Function, sizeof(LPVOID), old, &old);
            thunk++;
        }
        import++;
    }
}

int main() {
    zero_iat();
    std::cout << "IAT entries zeroed (may crash app depending on use)." << std::endl;
    return 0;
}

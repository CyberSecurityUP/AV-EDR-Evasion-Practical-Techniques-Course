// Filename: inspect_pe_sections.cpp
// Purpose: Inspect PE sections (.text, .data, .rdata) for understanding payload placement.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

void PrintSections(const char* filename) {
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return;

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID base = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)base + dos->e_lfanew);
    PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(nt);

    std::cout << "Sections found in " << filename << ":\n";
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        std::cout << std::string((char*)sections[i].Name, 8) << " - RVA: 0x" 
                  << std::hex << sections[i].VirtualAddress << " | Size: 0x" 
                  << sections[i].Misc.VirtualSize << "\n";
    }

    UnmapViewOfFile(base);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

int main() {
    PrintSections("C:\\Windows\\System32\\notepad.exe");
    return 0;
}

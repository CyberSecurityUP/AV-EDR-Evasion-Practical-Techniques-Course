// Filename: obfuscate_iocs.cpp
// Purpose: Encode strings (e.g., C2 domains, API names) to hide them in memory.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>

void xor_decrypt(char* data, size_t len, char key) {
    for (size_t i = 0; i < len; ++i)
        data[i] ^= key;
}

int main() {
    char data[] = { 'h','t','t','p',':','/','/','1','9','2','.','1','6','8','.','1','.','1','\0' };
    xor_decrypt(data, sizeof(data) - 1, 0x13);  // Encrypt
    xor_decrypt(data, sizeof(data) - 1, 0x13);  // Decrypt before use

    std::cout << "C2 Address: " << data << std::endl;
    return 0;
}

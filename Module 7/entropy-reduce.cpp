// Filename: entropy_reduce.cpp
// Purpose: Mimic low entropy payload to bypass ML/AV
// Instructor: Joas A Santos

#include <iostream>
#include <cstring>

char* LowEntropyPayload() {
    static char encoded[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; // Fake low entropy
    return encoded;
}

int main() {
    char* data = LowEntropyPayload();
    std::cout << "Payload length: " << strlen(data) << "\n";
    return 0;
}

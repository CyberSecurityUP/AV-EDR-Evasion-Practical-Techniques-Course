// Filename: anti_sandbox.cpp
// Purpose: Detect sandbox via mouse, system uptime, and sleep skip
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>

bool DetectMouseMovement() {
    POINT p1, p2;
    GetCursorPos(&p1);
    Sleep(2000);
    GetCursorPos(&p2);
    return (p1.x == p2.x && p1.y == p2.y);
}

bool DetectSleepSkip() {
    DWORD start = GetTickCount();
    Sleep(2000);
    DWORD end = GetTickCount();
    return (end - start < 1500); // Skipped
}

int main() {
    if (DetectMouseMovement() || DetectSleepSkip()) {
        std::cout << "Sandbox detected. Exiting.\n";
        return 1;
    }

    std::cout << "Environment seems clean.\n";
    return 0;
}

// Filename: custom_delay.cpp
// Purpose: Anti-sandbox via multi-stage sleep verification
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>

bool TimeCheck() {
    ULONGLONG start = GetTickCount64();
    Sleep(3000);
    return (GetTickCount64() - start > 2500);
}

int main() {
    if (!TimeCheck()) {
        std::cout << "Likely sandbox skipping sleep.\n";
        return 1;
    }

    std::cout << "Delay executed properly.\n";
    return 0;
}

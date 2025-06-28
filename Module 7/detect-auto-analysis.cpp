// Filename: detect_auto_analysis.cpp
// Purpose: Detect Hybrid-Analysis, Any.Run, Cuckoo, etc.
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>

bool DetectUsernames() {
    char username[UNLEN + 1];
    DWORD len = UNLEN + 1;
    GetUserNameA(username, &len);

    const char* analysisUsers[] = { "cuckoo", "user", "sandbox", "malware" };
    for (const auto& u : analysisUsers) {
        if (strstr(username, u)) return true;
    }

    return false;
}

bool DetectAnalysisWindow() {
    return FindWindowA(NULL, "Cuckoo Agent") != NULL;
}

int main() {
    if (DetectUsernames() || DetectAnalysisWindow()) {
        std::cout << "Analysis Platform Detected.\n";
        return 1;
    }

    std::cout << "Environment OK.\n";
    return 0;
}

// Filename: detect_hw_bp.cpp
// Purpose: Detect hardware breakpoints in debug registers
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>

bool CheckHardwareBreakpoints() {
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    HANDLE hThread = GetCurrentThread();

    if (GetThreadContext(hThread, &ctx)) {
        return (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3);
    }

    return false;
}

int main() {
    if (CheckHardwareBreakpoints()) {
        std::cout << "Hardware breakpoint detected.\n";
        return 1;
    }

    std::cout << "No hardware breakpoints.\n";
    return 0;
}

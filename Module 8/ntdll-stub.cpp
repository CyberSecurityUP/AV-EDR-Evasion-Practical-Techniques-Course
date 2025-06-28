// Filename: ntdll_stub.c
// Purpose: Custom syscall stub for NtWriteVirtualMemory
// Instructor: Joas A Santos

#include <windows.h>
#include <stdio.h>

typedef NTSTATUS(NTAPI* pNtWriteVirtualMemory)(
    HANDLE, PVOID, PVOID, ULONG, PULONG
);

unsigned char stub[] = {
    0x4C, 0x8B, 0xD1,             // mov r10, rcx
    0xB8, 0x3A, 0x00, 0x00, 0x00, // mov eax, 0x3A (syscall number for NtWriteVirtualMemory)
    0x0F, 0x05,                   // syscall
    0xC3                          // ret
};

int main() {
    void* exec = VirtualAlloc(NULL, sizeof(stub), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, stub, sizeof(stub));

    pNtWriteVirtualMemory NtStub = (pNtWriteVirtualMemory)exec;
    printf("[+] Custom syscall stub ready at %p\n", NtStub);
    return 0;
}

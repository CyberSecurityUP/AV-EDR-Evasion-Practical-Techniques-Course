// Filename: anti_vm.cpp
// Purpose: Detect VMs using MAC and registry keys
// Instructor: Joas A Santos

#include <windows.h>
#include <iostream>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

bool CheckMacPrefix() {
    PIP_ADAPTER_INFO AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG buflen = sizeof(IP_ADAPTER_INFO);

    if (GetAdaptersInfo(AdapterInfo, &buflen) == ERROR_BUFFER_OVERFLOW) {
        AdapterInfo = (IP_ADAPTER_INFO*)malloc(buflen);
        GetAdaptersInfo(AdapterInfo, &buflen);
    }

    BYTE* mac = AdapterInfo->Address;
    // Common VM MACs: VMWare: 00-05-69, VirtualBox: 08-00-27
    if ((mac[0] == 0x00 && mac[1] == 0x05 && mac[2] == 0x69) ||
        (mac[0] == 0x08 && mac[1] == 0x00 && mac[2] == 0x27)) {
        return true;
    }

    return false;
}

int main() {
    if (CheckMacPrefix()) {
        std::cout << "VM Detected.\n";
        return 1;
    }

    std::cout << "Physical Machine.\n";
    return 0;
}

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "patterntools.h"

int main()
{
    SigScanner Sig;

    HANDLE procID = Sig.GetProcess(L"ac_client.exe");
    uintptr_t module = Sig.GetModule(procID, L"ac_client.exe");
    DWORD Target = Sig.TargetID;
    HANDLE TargetProcess = Sig.TargetProcess;
    uintptr_t health = 0xEC;
    uintptr_t HealthWithModule;
    uintptr_t localPlayerWithModule;
    uintptr_t localPlayer = 0x50F4F4;
    uintptr_t playerxhealth;
    DWORD newVal = 1337;

    while (procID == 0)
        continue;

    if (procID != 0)
    {
        std::cout << "Successfully located the specified process " << std::endl;
        if (module != 0)
        {
            std::cout << "Successfully located the specified module " << std::endl;
            std::cout << "loware can now be launched " << std::endl;
            
            ReadProcessMemory(TargetProcess, (LPVOID)(module + localPlayer), &localPlayerWithModule, sizeof(uintptr_t), 0);
            ReadProcessMemory(TargetProcess, (LPVOID)(module + health), &HealthWithModule, sizeof(uintptr_t), 0);
            ReadProcessMemory(TargetProcess, (LPVOID)(localPlayerWithModule + HealthWithModule), &playerxhealth, sizeof(uintptr_t), 0);
            
            if (WriteProcessMemory(TargetProcess, (LPVOID)(playerxhealth), &newVal, sizeof(DWORD), 0))
            {
                std::cout << "Successfully changed the memory value " << std::endl;
            }
            else
            {
                std::cerr << "Failed to change the memory value " << std::endl;
            }
        }
    }
    
    
    
    return 0;
}
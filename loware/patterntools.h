#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>


class SigScanner
{
public:

	DWORD TargetID;
	HANDLE TargetProcess;
	uintptr_t TargetModule;

	HANDLE GetProcess(const wchar_t* procName)
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnap != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 procEntry;
			procEntry.dwSize = sizeof(procEntry);

			if (Process32First(hSnap, &procEntry))
			{
				do
				{
					if (!_wcsicmp(procEntry.szExeFile, procName))
					{
						TargetID = procEntry.th32ProcessID;
						CloseHandle(hSnap);
						TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, TargetID);
						if (TargetProcess != NULL)
							return TargetProcess;
						break;
					}

				} while (Process32Next(hSnap, &procEntry));
			}
		}
		return 0;
	}

	uintptr_t GetModule(HANDLE procId, const wchar_t* modName)
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE and TH32CS_SNAPMODULE32, TargetID);

		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);

			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!_wcsicmp(modEntry.szModule, modName))
					{
						CloseHandle(hSnap);
						TargetModule = (DWORD)modEntry.modBaseSize + (DWORD)modEntry.hModule;
						if (TargetModule != 0)
							return TargetModule;
						break;
					}

				} while (Module32Next(hSnap, &modEntry));
			}
		}
	}

	template <typename var>

	var ReadMemory(HANDLE TargetProcess, uintptr_t module, uintptr_t Address, uintptr_t newValue)
	{
		return ReadProcessMemory(TargetProcess, (LPVOID)(module + Address), &newValue, sizeof(var), 0);
	}

	template <typename var>

	var ReadAddressWithAddress(HANDLE TargetProcess, uintptr_t FirstAddress, uintptr_t SecondAddress, uintptr_t newValue)
	{
		return ReadProcessMemory(TargetProcess, (LPVOID)(FirstAddress + SecondAddress), &newValue, sizeof(var), 0);
	}

	template <typename var>

	var WriteMemory(HANDLE TargetProcess, uintptr_t Address, DWORD newValue)
	{
		return WriteProcessMemory(TargetProcess, (LPVOID)(Address), &newValue, sizeof(var), 0);
	}
	
};

#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cinttypes>

extern HANDLE hProcess;

namespace WinApi
{


    const inline DWORD GetProcessId(const wchar_t* processName) {
        DWORD processId = 0;

        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return 0;
        }

        PROCESSENTRY32W processEntry{};
        processEntry.dwSize = sizeof(processEntry);

        if (Process32FirstW(snapshotHandle, &processEntry)) {
            do {
                if (_wcsicmp(processName, processEntry.szExeFile) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshotHandle, &processEntry));
        }

        CloseHandle(snapshotHandle);
        return processId;
    }

    const inline std::uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
        std::uintptr_t moduleBase = 0;

        HANDLE snapshotHandle =
            CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return 0;
        }

        MODULEENTRY32W moduleEntry{};
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(snapshotHandle, &moduleEntry)) {
            do {
                if (_wcsicmp(moduleName, moduleEntry.szModule) == 0) {
                    moduleBase = reinterpret_cast<std::uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(snapshotHandle, &moduleEntry));
        }

        CloseHandle(snapshotHandle);
        return moduleBase;
    }

    template <typename T>
    const inline T Read(const std::uintptr_t& address) {
        T value{};
        ReadProcessMemory(
            hProcess,
            reinterpret_cast<LPCVOID>(address),
            &value,
            sizeof(T),
            nullptr
        );
        return value;
    }

    const inline bool ReadInto(const std::uintptr_t address, void* dst, const size_t bytes) {
        SIZE_T bytesRead = 0;
        const BOOL ok = ReadProcessMemory(
            hProcess,
            reinterpret_cast<LPCVOID>(address),
            dst,
            bytes,
            &bytesRead
        );

        return (ok != FALSE && bytesRead == bytes);
    }

    template <typename U>
    const inline void Write(const uintptr_t& address, const U& value)
    {
        WriteProcessMemory(hProcess, reinterpret_cast<void*>(address), &value, sizeof(U), 0);
    }


    inline void ReadString(const uintptr_t& address, char* buffer, SIZE_T size) {
        ReadProcessMemory(hProcess, reinterpret_cast<const void*>(address), buffer, size, 0);
        buffer[size - 1] = '\0'; // Ensure null-termination
    }

}


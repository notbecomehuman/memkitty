#pragma once

#include "../common/process-architecture.h"
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>

class Process
{
public:
    explicit Process(
        DWORD pid,
        ProcessArchitecture architecture =
            ProcessArchitecture::Auto
    );
    ~Process();

    bool Open();
    void Close();

    bool IsOpen() const;

    HANDLE GetHandle() const;
    DWORD GetPid() const;

    uintptr_t GetBaseAddress() const;

    static std::vector<DWORD> GetPidsByName(
        const std::wstring& processName
    );

    ProcessArchitecture GetArchitecture() const;
private:
    bool DetectArchitecture();

private:
    DWORD pid;
    HANDLE handle;

    ProcessArchitecture requestedArchitecture;
    ProcessArchitecture architecture;
};
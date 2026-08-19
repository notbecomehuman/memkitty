#pragma once

#include "../common/process-architecture.h"
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_set>


struct FindPidsData
{
    std::wstring title;
    std::unordered_set<DWORD> pids;
};

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
    static std::vector<DWORD> GetPidsByTitle(
        const std::wstring& windowTitle
    );
    /* static std::vector<DWORD> GetHwndByPid(
        const std::wstring& windowTitle
    );
    std::vector<DWORD> PostMessage(
        const std::wstring& windowTitle
    ); */

    static BOOL CALLBACK EnumWindowsForPids(HWND hwnd, LPARAM lParam);

    ProcessArchitecture GetArchitecture() const;
private:
    bool DetectArchitecture();

private:
    DWORD pid;
    HANDLE handle;

    ProcessArchitecture requestedArchitecture;
    ProcessArchitecture architecture;
};
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

struct FindWindowData
{
    DWORD pid;
    HWND hwnd;
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
    static HWND GetHwndByPid(DWORD pid);
    static bool PostMessage(
        HWND hwnd,
        UINT message,
        WPARAM wParam = 0,
        LPARAM lParam = 0
    );
    static BOOL CALLBACK EnumWindowsForPid(HWND hwnd, LPARAM lParam);
    static BOOL CALLBACK EnumWindowsForTitle(HWND hwnd, LPARAM lParam);

    ProcessArchitecture GetArchitecture() const;
private:
    bool DetectArchitecture();

private:
    DWORD pid;
    HANDLE handle;

    ProcessArchitecture requestedArchitecture;
    ProcessArchitecture architecture;
};
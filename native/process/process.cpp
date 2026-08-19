#include "process.h"

#include <TlHelp32.h>

Process::Process(
    DWORD pid,
    ProcessArchitecture architecture
)
    : pid(pid),
      handle(nullptr),
      requestedArchitecture(architecture),
      architecture(ProcessArchitecture::Auto)
{
}

Process::~Process()
{
    Close();
}

ProcessArchitecture
Process::GetArchitecture() const
{
    return architecture;
}

bool Process::DetectArchitecture()
{
    BOOL isWow64 = FALSE;

    if (!IsWow64Process(
        handle,
        &isWow64
    ))
    {
        return false;
    }

#ifdef _WIN64

    if (isWow64)
    {
        architecture =
            ProcessArchitecture::X86;
    }
    else
    {
        architecture =
            ProcessArchitecture::X64;
    }

#else

    architecture =
        ProcessArchitecture::X86;

#endif

    return true;
}

bool Process::Open()
{
    if (handle != nullptr)
    {
        return true;
    }

    handle = OpenProcess(
        PROCESS_VM_READ |
        PROCESS_VM_WRITE |
        PROCESS_VM_OPERATION |
        PROCESS_QUERY_INFORMATION,
        FALSE,
        pid
    );

    if (handle == nullptr)
    {
        return false;
    }

    if (
        requestedArchitecture !=
        ProcessArchitecture::Auto
    )
    {
        architecture =
            requestedArchitecture;

        return true;
    }

    if (!DetectArchitecture())
    {
        Close();
        return false;
    }

    return true;
}

void Process::Close()
{
    if (handle != nullptr)
    {
        CloseHandle(handle);
        handle = nullptr;
    }

    architecture =
        ProcessArchitecture::Auto;
}

bool Process::IsOpen() const
{
    return handle != nullptr;
}

HANDLE Process::GetHandle() const
{
    return handle;
}

DWORD Process::GetPid() const
{
    return pid;
}

uintptr_t Process::GetBaseAddress() const
{
    uintptr_t baseAddress = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
        pid
    );

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    MODULEENTRY32 moduleEntry{};
    moduleEntry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &moduleEntry))
    {
        baseAddress =
            reinterpret_cast<uintptr_t>(
                moduleEntry.modBaseAddr
            );
    }

    CloseHandle(snapshot);

    return baseAddress;
}

std::vector<DWORD> Process::GetPidsByName(
    const std::wstring& processName
)
{
    std::vector<DWORD> pids;

    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0
    );

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return pids;
    }

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(
                    processName.c_str(),
                    entry.szExeFile
                ) == 0)
            {
                pids.push_back(
                    entry.th32ProcessID
                );
            }
        }
        while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);

    return pids;
}

BOOL CALLBACK Process::EnumWindowsForPids(HWND hwnd, LPARAM lParam)
{
    auto* data = reinterpret_cast<FindPidsData*>(lParam);

    if (!IsWindowVisible(hwnd))
        return TRUE;

    const int length = GetWindowTextLengthW(hwnd);

    if (length <= 0)
        return TRUE;

    std::wstring windowTitle(length + 1, L'\0');

    GetWindowTextW(
        hwnd,
        windowTitle.data(),
        length + 1
    );

    windowTitle.resize(length);

    if (windowTitle != data->title)
        return TRUE;

    DWORD pid = 0;

    GetWindowThreadProcessId(
        hwnd,
        &pid
    );

    if (pid != 0)
        data->pids.insert(pid);

    return TRUE;
}

std::vector<DWORD> Process::GetPidsByTitle(const std::wstring& windowTitle)
{
    FindPidsData data;
    data.title = windowTitle;

    EnumWindows(
        EnumWindowsForPids,
        reinterpret_cast<LPARAM>(&data)
    );

    return std::vector<DWORD>(
        data.pids.begin(),
        data.pids.end()
    );
}
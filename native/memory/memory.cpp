#include "memory.h"
#include <vector>

bool Memory::ReadInt32(
    HANDLE process,
    uintptr_t address,
    int32_t& value
)
{
    SIZE_T bytesRead = 0;

    return ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        &value,
        sizeof(value),
        &bytesRead
    ) &&
    bytesRead == sizeof(value);
}

bool Memory::ReadInt64(
    HANDLE process,
    uintptr_t address,
    int64_t& value
)
{
    SIZE_T bytesRead = 0;

    return ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        &value,
        sizeof(value),
        &bytesRead
    ) &&
    bytesRead == sizeof(value);
}

bool Memory::ReadFloat(
    HANDLE process,
    uintptr_t address,
    float& value
)
{
    SIZE_T bytesRead = 0;

    return ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        &value,
        sizeof(value),
        &bytesRead
    ) &&
    bytesRead == sizeof(value);
}

bool Memory::ReadWString(
    HANDLE process,
    uintptr_t address,
    std::wstring& value,
    size_t maxLength
)
{
    std::vector<wchar_t> buffer(
        maxLength + 1
    );

    SIZE_T bytesRead = 0;

    BOOL result = ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        buffer.data(),
        buffer.size() * sizeof(wchar_t),
        &bytesRead
    );

    if (!result)
    {
        return false;
    }

    buffer[maxLength] = L'\0';

    value = buffer.data();

    return true;
}

bool Memory::ReadUInt32(
    HANDLE process,
    uintptr_t address,
    uint32_t& value
)
{
    SIZE_T bytesRead = 0;

    BOOL result = ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        &value,
        sizeof(value),
        &bytesRead
    );

    return result &&
           bytesRead == sizeof(value);
}

bool Memory::ReadUInt64(
    HANDLE process,
    uintptr_t address,
    uint64_t& value
)
{
    SIZE_T bytesRead = 0;

    BOOL result = ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        &value,
        sizeof(value),
        &bytesRead
    );

    return result &&
           bytesRead == sizeof(value);
}

bool Memory::ReadPointer(
    HANDLE process,
    uintptr_t address,
    ProcessArchitecture architecture,
    uintptr_t& value
)
{
    SIZE_T bytesRead = 0;

    if (architecture == ProcessArchitecture::X86)
    {
        uint32_t pointerValue = 0;

        BOOL result = ReadProcessMemory(
            process,
            reinterpret_cast<LPCVOID>(address),
            &pointerValue,
            sizeof(pointerValue),
            &bytesRead
        );

        if (
            !result ||
            bytesRead != sizeof(pointerValue)
        )
        {
            return false;
        }

        value =
            static_cast<uintptr_t>(
                pointerValue
            );

        return true;
    }

    if (architecture == ProcessArchitecture::X64)
    {
        uint64_t pointerValue = 0;

        BOOL result = ReadProcessMemory(
            process,
            reinterpret_cast<LPCVOID>(address),
            &pointerValue,
            sizeof(pointerValue),
            &bytesRead
        );

        if (
            !result ||
            bytesRead != sizeof(pointerValue)
        )
        {
            return false;
        }

        value =
            static_cast<uintptr_t>(
                pointerValue
            );

        return true;
    }

    return false;
}

bool Memory::ReadBytes(
    HANDLE process,
    uintptr_t address,
    size_t size,
    std::vector<uint8_t>& buffer
)
{
    buffer.resize(size);

    SIZE_T bytesRead = 0;

    BOOL result = ReadProcessMemory(
        process,
        reinterpret_cast<LPCVOID>(address),
        buffer.data(),
        size,
        &bytesRead
    );

    if (!result)
    {
        buffer.clear();
        return false;
    }

    buffer.resize(bytesRead);

    return bytesRead == size;
}

uintptr_t Memory::AllocateMemory(
        HANDLE process,
        size_t size
)
{
    uintptr_t address = (uintptr_t)VirtualAllocEx(process, NULL, size, MEM_COMMIT, PAGE_READWRITE);

    if (address == 0) return 0;

    return address;
}

bool Memory::InjectAndExecute(
    HANDLE process,
    uintptr_t allocatedMemoryAddress,
    const char* code,
    size_t size
)
{
    HANDLE hProcThread;

    WriteProcessMemory(process, reinterpret_cast<LPVOID>(allocatedMemoryAddress), code, size, NULL);

    hProcThread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)allocatedMemoryAddress, NULL, NULL, NULL);
    if (hProcThread == INVALID_HANDLE_VALUE) return false;

    WaitForSingleObject(hProcThread, INFINITE);
    CloseHandle(hProcThread);
    return true;
}

bool Memory::Inject(
    HANDLE process,
    uintptr_t allocatedMemoryAddress,
    const char* code,
    size_t size
)
{
    bool result = WriteProcessMemory(process, reinterpret_cast<LPVOID>(allocatedMemoryAddress), code, size, NULL);

    return result;
}


#pragma once

#include "../common/process-architecture.h"
#include <Windows.h>
#include <string>
#include <cstdint>
#include <vector>

class Memory
{
public:
    static uintptr_t AllocateMemory(
        HANDLE process,
        size_t size
    );

    static bool InjectAndExecute(
        HANDLE process,
        uintptr_t allocatedMemoryAddress,
        const char* code,
        size_t size
    );

    static bool Inject(
        HANDLE process,
        uintptr_t allocatedMemoryAddress,
        const char* code,
        size_t size
    );

    static bool ReadInt32(
        HANDLE process,
        uintptr_t address,
        int32_t& value
    );

    static bool ReadInt64(
        HANDLE process,
        uintptr_t address,
        int64_t& value
    );

    static bool ReadFloat(
        HANDLE process,
        uintptr_t address,
        float& value
    );

    static bool ReadWString(
        HANDLE process,
        uintptr_t address,
        std::wstring& value,
        size_t maxLength
    );

    static bool ReadUInt32(
         HANDLE process,
         uintptr_t address,
         uint32_t& value
    );

    static bool ReadUInt64(
        HANDLE process,
        uintptr_t address,
        uint64_t& value
    );

    static bool ReadPointer(
        HANDLE process,
        uintptr_t address,
        ProcessArchitecture architecture,
        uintptr_t& value
    );

    static bool ReadBytes(
        HANDLE process,
        uintptr_t address,
        size_t size,
        std::vector<uint8_t>& buffer
    );
};
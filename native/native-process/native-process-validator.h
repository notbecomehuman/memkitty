#pragma once

#include <napi.h>

#include <cstdint>
#include <initializer_list>

class Process;

enum class ArgumentType
{
    Number,
    BigInt,
    String,
    Boolean,
    Buffer,
    Address
};

struct ValidationOptions
{
    bool requireOpenProcess = false;
};

class NativeProcessValidator
{
public:
    static bool Validate(
        Process* process,
        const Napi::CallbackInfo& info,
        std::initializer_list<ArgumentType> arguments,
        ValidationOptions options = {}
    );

    static bool TryGetAddress(
        const Napi::Value& value,
        uintptr_t& address
    );
};
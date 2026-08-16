#include "native-process-validator.h"

#include "../process/process.h"

#include <cstdint>
#include <climits>

bool NativeProcessValidator::TryGetAddress(
    const Napi::Value& value,
    uintptr_t& address
)
{
    if (!value.IsBigInt())
    {
        return false;
    }

    bool lossless = false;

    uint64_t rawAddress =
        value
            .As<Napi::BigInt>()
            .Uint64Value(
                &lossless
            );

    if (!lossless)
    {
        return false;
    }

    if (
        rawAddress >
        static_cast<uint64_t>(
            UINTPTR_MAX
        )
    )
    {
        return false;
    }

    address =
        static_cast<uintptr_t>(
            rawAddress
        );

    return true;
}

bool NativeProcessValidator::Validate(
    Process* process,
    const Napi::CallbackInfo& info,
    std::initializer_list<ArgumentType> arguments,
    ValidationOptions options
)
{
    Napi::Env env = info.Env();

    if (
        options.requireOpenProcess &&
        (
            process == nullptr ||
            !process->IsOpen()
        )
    )
    {
        Napi::Error::New(
            env,
            "Process is not open"
        ).ThrowAsJavaScriptException();

        return false;
    }

    if (
        info.Length() != arguments.size()
    )
    {
        Napi::TypeError::New(
            env,
            "Invalid argument count"
        ).ThrowAsJavaScriptException();

        return false;
    }

    size_t index = 0;

    for (auto expectedType : arguments)
    {
        const Napi::Value& value =
            info[index];

        bool valid = false;

        switch (expectedType)
        {
            case ArgumentType::Number:
                valid = value.IsNumber();
                break;

            case ArgumentType::BigInt:
                valid = value.IsBigInt();
                break;

            case ArgumentType::String:
                valid = value.IsString();
                break;

            case ArgumentType::Boolean:
                valid = value.IsBoolean();
                break;

            case ArgumentType::Buffer:
                valid = value.IsBuffer();
                break;

            case ArgumentType::Address:
            {
                uintptr_t address;

                valid =
                    TryGetAddress(
                        value,
                        address
                    );

                break;
            }
        }

        if (!valid)
        {
            Napi::TypeError::New(
                env,
                "Invalid argument type"
            ).ThrowAsJavaScriptException();

            return false;
        }

        ++index;
    }

    return true;
}
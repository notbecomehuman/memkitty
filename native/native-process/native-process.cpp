#include "native-process.h"

#include "../process/process.h"
#include "../memory/memory.h"
#include "native-process-validator.h"

#include <vector>
#include <string>
#include <cstdint>


Napi::FunctionReference NativeProcess::constructor;


// ============================================================
// Constructor / Destructor
// ============================================================

NativeProcess::NativeProcess(
    const Napi::CallbackInfo& info
)
    : Napi::ObjectWrap<NativeProcess>(info),
      process(nullptr)
{
    Napi::Env env = info.Env();

    if (
        info.Length() < 1 ||
        !info[0].IsNumber()
    )
    {
        Napi::TypeError::New(
            env,
            "PID must be a number"
        ).ThrowAsJavaScriptException();

        return;
    }

    DWORD pid =
        info[0]
            .As<Napi::Number>()
            .Uint32Value();

    ProcessArchitecture architecture =
        ProcessArchitecture::Auto;

    // Второй аргумент необязательный
    if (info.Length() >= 2)
    {
        if (!info[1].IsString())
        {
            Napi::TypeError::New(
                env,
                "Architecture must be a string"
            ).ThrowAsJavaScriptException();

            return;
        }

        std::string value =
            info[1]
                .As<Napi::String>()
                .Utf8Value();

        if (value == "auto")
        {
            architecture =
                ProcessArchitecture::Auto;
        }
        else if (value == "x86")
        {
            architecture =
                ProcessArchitecture::X86;
        }
        else if (value == "x64")
        {
            architecture =
                ProcessArchitecture::X64;
        }
        else
        {
            Napi::TypeError::New(
                env,
                "Architecture must be auto, x86 or x64"
            ).ThrowAsJavaScriptException();

            return;
        }
    }

    process =
        new Process(
            pid,
            architecture
        );
}


NativeProcess::~NativeProcess()
{
    delete process;
    process = nullptr;
}


// ============================================================
// Init
// ============================================================

Napi::Object NativeProcess::Init(
    Napi::Env env,
    Napi::Object exports
)
{
    Napi::Function func = DefineClass(
        env,
        "NativeProcess",
        {
            InstanceMethod(
                "open",
                &NativeProcess::Open
            ),

            InstanceMethod(
                "close",
                &NativeProcess::Close
            ),

            InstanceMethod(
                "isOpen",
                &NativeProcess::IsOpen
            ),

            InstanceMethod(
                "getBaseAddress",
                &NativeProcess::GetBaseAddress
            ),

            InstanceMethod(
                "readInt32",
                &NativeProcess::ReadInt32
            ),

            InstanceMethod(
                "readInt64",
                &NativeProcess::ReadInt64
            ),

            InstanceMethod(
                "readUInt32",
                &NativeProcess::ReadUInt32
            ),

            InstanceMethod(
                "readUInt64",
                &NativeProcess::ReadUInt64
            ),

            InstanceMethod(
                "readFloat",
                &NativeProcess::ReadFloat
            ),

            InstanceMethod(
                "readWString",
                &NativeProcess::ReadWString
            ),

            InstanceMethod(
                "readPointer",
                &NativeProcess::ReadPointer
            ),

            InstanceMethod(
                "readBytes",
                &NativeProcess::ReadBytes
            ),

            InstanceMethod(
                "allocateMemory",
                &NativeProcess::AllocateMemory
            ),

            InstanceMethod(
                "injectAndExecute",
                &NativeProcess::InjectAndExecute
            ),

            InstanceMethod(
                "inject",
                &NativeProcess::Inject
            )
        }
    );

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set(
        "NativeProcess",
        func
    );

    return exports;
}


// ============================================================
// Process
// ============================================================

Napi::Value NativeProcess::Open(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {}
    ))
    {
        return env.Null();
    }

    if (process == nullptr)
    {
        Napi::Error::New(
            env,
            "Process is not initialized"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    bool result = process->Open();

    return Napi::Boolean::New(
        env,
        result
    );
}


Napi::Value NativeProcess::Close(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {}
    ))
    {
        return env.Null();
    }

    if (process != nullptr)
    {
        process->Close();
    }

    return env.Undefined();
}


Napi::Value NativeProcess::IsOpen(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {}
    ))
    {
        return env.Null();
    }

    bool isOpen =
        process != nullptr &&
        process->IsOpen();

    return Napi::Boolean::New(
        env,
        isOpen
    );
}


Napi::Value NativeProcess::GetBaseAddress(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {},
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t baseAddress =
        process->GetBaseAddress();

    if (baseAddress == 0)
    {
        Napi::Error::New(
            env,
            "Failed to get process base address"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::BigInt::New(
        env,
        static_cast<uint64_t>(
            baseAddress
        )
    );
}


// ============================================================
// Read Int32
// ============================================================

Napi::Value NativeProcess::ReadInt32(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    int32_t value = 0;

    if (!Memory::ReadInt32(
        process->GetHandle(),
        address,
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadInt32 failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::Number::New(
        env,
        value
    );
}


// ============================================================
// Read UInt32
// ============================================================

Napi::Value NativeProcess::ReadUInt32(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    uint32_t value = 0;

    if (!Memory::ReadUInt32(
        process->GetHandle(),
        address,
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadUInt32 failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::Number::New(
        env,
        value
    );
}


// ============================================================
// Read Int64
// ============================================================

Napi::Value NativeProcess::ReadInt64(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    int64_t value = 0;

    if (!Memory::ReadInt64(
        process->GetHandle(),
        address,
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadInt64 failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::BigInt::New(
        env,
        value
    );
}


// ============================================================
// Read UInt64
// ============================================================

Napi::Value NativeProcess::ReadUInt64(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    uint64_t value = 0;

    if (!Memory::ReadUInt64(
        process->GetHandle(),
        address,
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadUInt64 failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::BigInt::New(
        env,
        value
    );
}


// ============================================================
// Read Float
// ============================================================

Napi::Value NativeProcess::ReadFloat(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    float value = 0.0f;

    if (!Memory::ReadFloat(
        process->GetHandle(),
        address,
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadFloat failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::Number::New(
        env,
        value
    );
}


// ============================================================
// Read Pointer
// ============================================================

Napi::Value NativeProcess::ReadPointer(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    uintptr_t value = 0;

    if (!Memory::ReadPointer(
        process->GetHandle(),
        address,
        process->GetArchitecture(),
        value
    ))
    {
        Napi::Error::New(
            env,
            "ReadPointer failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::BigInt::New(
        env,
        static_cast<uint64_t>(
            value
        )
    );
}


// ============================================================
// Read WString
// ============================================================

Napi::Value NativeProcess::ReadWString(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address,
            ArgumentType::Number
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    uint32_t length =
        info[1]
            .As<Napi::Number>()
            .Uint32Value();

    if (length == 0)
    {
        return Napi::String::New(
            env,
            ""
        );
    }

    std::wstring value;

    if (!Memory::ReadWString(
        process->GetHandle(),
        address,
        value,
        static_cast<size_t>(length)
    ))
    {
        Napi::Error::New(
            env,
            "ReadWString failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    std::u16string utf16(
        reinterpret_cast<const char16_t*>(
            value.data()
        ),
        value.size()
    );

    return Napi::String::New(
        env,
        utf16
    );
}


// ============================================================
// Read Bytes
// ============================================================

Napi::Value NativeProcess::ReadBytes(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address,
            ArgumentType::Number
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t address = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        address
    ))
    {
        return env.Null();
    }

    uint32_t requestedSize =
        info[1]
            .As<Napi::Number>()
            .Uint32Value();

    if (requestedSize == 0)
    {
        return Napi::Buffer<uint8_t>::New(
            env,
            0
        );
    }

    size_t size =
        static_cast<size_t>(
            requestedSize
        );

    std::vector<uint8_t> buffer;

    if (!Memory::ReadBytes(
        process->GetHandle(),
        address,
        size,
        buffer
    ))
    {
        Napi::Error::New(
            env,
            "ReadBytes failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::Buffer<uint8_t>::Copy(
        env,
        buffer.data(),
        buffer.size()
    );
}


// ============================================================
// Allocate Memory
// ============================================================

Napi::Value NativeProcess::AllocateMemory(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Number
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uint32_t requestedSize =
        info[0]
            .As<Napi::Number>()
            .Uint32Value();

    if (requestedSize == 0)
    {
        Napi::RangeError::New(
            env,
            "Memory size must be greater than zero"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    size_t size =
        static_cast<size_t>(
            requestedSize
        );

    uintptr_t allocatedMemoryAddress =
        Memory::AllocateMemory(
            process->GetHandle(),
            size
        );

    if (allocatedMemoryAddress == 0)
    {
        Napi::Error::New(
            env,
            "AllocateMemory failed"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    return Napi::BigInt::New(
        env,
        static_cast<uint64_t>(
            allocatedMemoryAddress
        )
    );
}


// ============================================================
// Inject And Execute
// ============================================================

Napi::Value NativeProcess::InjectAndExecute(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address,
            ArgumentType::Buffer
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t allocatedMemoryAddress = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        allocatedMemoryAddress
    ))
    {
        return env.Null();
    }

    Napi::Buffer<uint8_t> buffer =
        info[1]
            .As<Napi::Buffer<uint8_t>>();

    const char* code =
        reinterpret_cast<const char*>(
            buffer.Data()
        );

    size_t len =
        buffer.Length();

    uintptr_t result =
        Memory::InjectAndExecute(
            process->GetHandle(),
            allocatedMemoryAddress,
            code,
            len
        );

    if (!result)
    {
        Napi::Error::New(
            env,
            "Inject and execute failed"
        ).ThrowAsJavaScriptException();

        return Napi::Boolean::New(
            env,
            false
        );
    }

    return Napi::Boolean::New(
        env,
        true
    );
}


// ============================================================
// Inject
// ============================================================

Napi::Value NativeProcess::Inject(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (!NativeProcessValidator::Validate(
        process,
        info,
        {
            ArgumentType::Address,
            ArgumentType::Buffer
        },
        ValidationOptions{ true }
    ))
    {
        return env.Null();
    }

    uintptr_t allocatedMemoryAddress = 0;

    if (!NativeProcessValidator::TryGetAddress(
        info[0],
        allocatedMemoryAddress
    ))
    {
        return env.Null();
    }

    Napi::Buffer<uint8_t> buffer =
        info[1]
            .As<Napi::Buffer<uint8_t>>();

    const char* code =
        reinterpret_cast<const char*>(
            buffer.Data()
        );

    size_t len =
        buffer.Length();

    uintptr_t result =
        Memory::Inject(
            process->GetHandle(),
            allocatedMemoryAddress,
            code,
            len
        );

    if (!result)
    {
        Napi::Error::New(
            env,
            "Inject failed"
        ).ThrowAsJavaScriptException();

        return Napi::Boolean::New(
            env,
            false
        );
    }

    return Napi::Boolean::New(
        env,
        true
    );
}
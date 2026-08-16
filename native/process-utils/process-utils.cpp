#include "process-utils.h"

#include "../process/process.h"

#include <string>
#include <vector>

Napi::Value GetPidsByName(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (
        info.Length() < 1 ||
        !info[0].IsString()
    )
    {
        Napi::TypeError::New(
            env,
            "Process name must be a string"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    std::u16string utf16 =
        info[0]
            .As<Napi::String>()
            .Utf16Value();

    std::wstring processName(
        reinterpret_cast<const wchar_t*>(
            utf16.data()
        ),
        utf16.size()
    );

    std::vector<DWORD> pids =
        Process::GetPidsByName(
            processName
        );

    Napi::Array result =
        Napi::Array::New(
            env,
            pids.size()
        );

    for (size_t i = 0; i < pids.size(); i++)
    {
        result.Set(
            static_cast<uint32_t>(i),
            Napi::Number::New(
                env,
                pids[i]
            )
        );
    }

    return result;
}

void RegisterProcessUtils(
    Napi::Env env,
    Napi::Object exports
)
{
    exports.Set(
        "getPidsByName",
        Napi::Function::New(
            env,
            GetPidsByName
        )
    );
}
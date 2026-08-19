#include "process-utils.h"

#include "../process/process.h"

#include <string>
#include <vector>

static std::wstring Utf8ToWide(const std::string& str)
{
    if (str.empty())
        return {};

    const int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        nullptr,
        0
    );

    std::wstring result(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        result.data(),
        size
    );

    return result;
}

Napi::Value GetPidsByNameBinding(
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

Napi::Value GetPidsByTitleBinding(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(
            env,
            "Window title must be a string"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    std::string titleUtf8 =
        info[0].As<Napi::String>().Utf8Value();

    std::wstring windowTitle =
        Utf8ToWide(titleUtf8);

    std::vector<DWORD> pids =
        Process::GetPidsByTitle(windowTitle);

    Napi::Array result =
        Napi::Array::New(env, pids.size());

    for (size_t i = 0; i < pids.size(); ++i)
    {
        result.Set(
            static_cast<uint32_t>(i),
            Napi::Number::New(env, pids[i])
        );
    }

    return result;
}

Napi::Value GetHwndByPidBinding(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(
            env,
            "pid must be a number"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    DWORD pid =
        info[0].As<Napi::Number>()
            .Uint32Value();

    HWND hwnd =
        Process::GetHwndByPid(pid);

    return Napi::BigInt::New(
        env,
        reinterpret_cast<uint64_t>(hwnd)
    );
}

Napi::Value PostMessageBinding(
    const Napi::CallbackInfo& info
)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(
            env,
            "Expected hwnd and message"
        ).ThrowAsJavaScriptException();

        return env.Null();
    }

    bool lossless = false;

    uint64_t hwndValue =
        info[0]
            .As<Napi::BigInt>()
            .Uint64Value(&lossless);

    UINT message =
        info[1]
            .As<Napi::Number>()
            .Uint32Value();

    WPARAM wParam = 0;
    LPARAM lParam = 0;

    if (info.Length() >= 3)
    {
        wParam =
            static_cast<WPARAM>(
                info[2]
                    .As<Napi::Number>()
                    .Int64Value()
            );
    }

    if (info.Length() >= 4)
    {
        lParam =
            static_cast<LPARAM>(
                info[3]
                    .As<Napi::Number>()
                    .Int64Value()
            );
    }

    bool result =
        Process::PostMessage(
            reinterpret_cast<HWND>(hwndValue),
            message,
            wParam,
            lParam
        );

    return Napi::Boolean::New(
        env,
        result
    );
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
            GetPidsByNameBinding
        )
    );
    exports.Set(
        "getPidsByTitle",
        Napi::Function::New(
            env,
            GetPidsByTitleBinding
        )
    );
    exports.Set(
        "getHwndByPid",
        Napi::Function::New(
            env,
            GetHwndByPidBinding
        )
    );
    exports.Set(
        "postMessage",
        Napi::Function::New(
            env,
            PostMessageBinding
        )
    );
}
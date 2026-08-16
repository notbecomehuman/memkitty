#pragma once

#include <napi.h>

class Process;
class NativeProcess : public Napi::ObjectWrap<NativeProcess>
{
public:
    static Napi::FunctionReference constructor;

    explicit NativeProcess(
        const Napi::CallbackInfo& info
    );

    ~NativeProcess();

    static Napi::Object Init(
        Napi::Env env,
        Napi::Object exports
    );

private:
    Process* process;

    Napi::Value Open(
        const Napi::CallbackInfo& info
    );

    Napi::Value Close(
        const Napi::CallbackInfo& info
    );

    Napi::Value IsOpen(
        const Napi::CallbackInfo& info
    );

    Napi::Value GetBaseAddress(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadInt32(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadInt64(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadUInt32(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadUInt64(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadFloat(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadPointer(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadBytes(
        const Napi::CallbackInfo& info
    );

    Napi::Value ReadWString(
        const Napi::CallbackInfo& info
    );

    Napi::Value AllocateMemory(
        const Napi::CallbackInfo& info
    );

    Napi::Value InjectAndExecute(
        const Napi::CallbackInfo& info
    );

    Napi::Value Inject(
        const Napi::CallbackInfo& info
    );
};
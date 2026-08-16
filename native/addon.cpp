#include <napi.h>

#include "native-process/native-process.h"
#include "process-utils/process-utils.h"

Napi::Object Init(
    Napi::Env env,
    Napi::Object exports
)
{
    NativeProcess::Init(
        env,
        exports
    );

    RegisterProcessUtils(
        env,
        exports
    );

    return exports;
}

NODE_API_MODULE(
    native_addon,
    Init
)
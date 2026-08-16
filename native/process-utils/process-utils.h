#pragma once

#include <napi.h>

Napi::Value GetPidsByName(
    const Napi::CallbackInfo& info
);

void RegisterProcessUtils(
    Napi::Env env,
    Napi::Object exports
);
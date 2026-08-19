#pragma once

#include <napi.h>

Napi::Value GetPidsByNameBinding(
    const Napi::CallbackInfo& info
);
Napi::Value GetPidsByTitleBinding(
    const Napi::CallbackInfo& info
);
Napi::Value GetHwndByPidBinding(
    const Napi::CallbackInfo& info
);
Napi::Value PostMessageBinding(
    const Napi::CallbackInfo& info
);

void RegisterProcessUtils(
    Napi::Env env,
    Napi::Object exports
);
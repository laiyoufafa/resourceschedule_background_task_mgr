/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_INTERFACES_KITS_NAPI_INCLUDE_COMMON_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_INTERFACES_KITS_NAPI_INCLUDE_COMMON_H

#include <string_ex.h>

#include "napi/native_api.h"
#include "napi/native_node_api.h"

#include "bgtaskmgr_inner_errors.h"
#include "delay_suspend_info.h"

namespace OHOS {
namespace BackgroundTaskMgr {
struct CallbackPromiseInfo {
    napi_ref callback = nullptr;
    napi_deferred deferred = nullptr;
    bool isCallback = false;
    int32_t errorCode = 0;
};

struct AsyncWorkData {
    explicit AsyncWorkData(napi_env napiEnv);
    virtual ~AsyncWorkData();
    napi_env env;
    napi_async_work asyncWork = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
};

class Common {
public:
    static napi_value NapiGetboolean(napi_env env, const bool &isValue);

    static napi_value NapiGetNull(napi_env env);

    static napi_value NapiGetUndefined(napi_env env);

    static napi_value GetCallbackErrorValue(napi_env env, int32_t errCode);

    static napi_value GetExpireCallbackValue(napi_env env, int32_t errCode, const napi_value &value);

    static void PaddingCallbackPromiseInfo(
        const napi_env &env, const napi_ref &callback, CallbackPromiseInfo &info, napi_value &promise);

    static void ReturnCallbackPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result);

    static void SetCallback(
        const napi_env &env, const napi_ref &callbackIn, const int32_t &errorCode, const napi_value &result);

    static void SetCallback(
        const napi_env &env, const napi_ref &callbackIn, const napi_value &result);

    static napi_value SetPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result);

    static napi_value JSParaError(const napi_env &env, const napi_ref &callback);

    static napi_value GetU16StringValue(const napi_env &env, const napi_value &value, std::u16string &result);

    static napi_value GetInt32NumberValue(const napi_env &env, const napi_value &value, int32_t &result);

    static napi_value SetDelaySuspendInfo(
        const napi_env &env, std::shared_ptr<DelaySuspendInfo>& delaySuspendInfo, napi_value &result);
};
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
#endif  // FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_INTERFACES_KITS_NAPI_INCLUDE_COMMON_H
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

#include "background_task_subscriber_stub.h"

#include <errors.h>
#include <ipc_skeleton.h>

#include "transient_task_log.h"
#include "background_task_subscriber_proxy.h"

namespace OHOS {
namespace BackgroundTaskMgr {
namespace {
constexpr int32_t APP_FIRST_UID = 10000;
}

BackgroundTaskSubscriberStub::BackgroundTaskSubscriberStub() {}
BackgroundTaskSubscriberStub::~BackgroundTaskSubscriberStub() {}

ErrCode BackgroundTaskSubscriberStub::OnRemoteRequest(uint32_t code,
    MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    std::u16string descriptor = BackgroundTaskSubscriberStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        BGTASK_LOGE("Local descriptor not match remote.");
        return ERR_TRANSACTION_FAILED;
    }

    auto uid = IPCSkeleton::GetCallingUid();
    if (uid >= APP_FIRST_UID) {
        BGTASK_LOGE("OnRemoteRequest failed, illegal calling uid %d.", uid);
        return ERR_INVALID_DATA;
    }

    switch (code) {
        case ON_CONNECTED: {
            return HandleOnConnected();
        }
        case ON_DISCONNECTED: {
            return HandleOnDisconnected();
        }
        case ON_TRANSIENT_TASK_START: {
            return HandleOnTransientTaskStart(data);
        }
        case ON_TRANSIENT_TASK_END: {
            return HandleOnTransientTaskEnd(data);
        }
        case ON_CONTINUOUS_TASK_START: {
            return HandleOnContinuousTaskStart(data);
        }
        case ON_CONTINUOUS_TASK_STOP: {
            return HandleOnContinuousTaskCancel(data);
        }
        default:
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
}

ErrCode BackgroundTaskSubscriberStub::HandleOnConnected()
{
    OnConnected();
    return ERR_OK;
}

ErrCode BackgroundTaskSubscriberStub::HandleOnDisconnected()
{
    OnDisconnected();
    return ERR_OK;
}

ErrCode BackgroundTaskSubscriberStub::HandleOnTransientTaskStart(MessageParcel& data)
{
    auto info = TransientTaskAppInfo::Unmarshalling(data);
    if (info == nullptr) {
        BGTASK_LOGE("Read parcel failed.");
        return ERR_INVALID_DATA;
    }
    OnTransientTaskStart(info);
    return ERR_NONE;
}

ErrCode BackgroundTaskSubscriberStub::HandleOnTransientTaskEnd(MessageParcel& data)
{
    auto info = TransientTaskAppInfo::Unmarshalling(data);
    if (info == nullptr) {
        BGTASK_LOGE("Read parcel failed.");
        return ERR_INVALID_DATA;
    }
    OnTransientTaskEnd(info);
    return ERR_NONE;
}

ErrCode BackgroundTaskSubscriberStub::HandleOnContinuousTaskStart(MessageParcel &data)
{
    BGTASK_LOGI("begin");
    std::shared_ptr<ContinuousTaskCallbackInfo> continuousTaskCallbackInfo
        = std::shared_ptr<ContinuousTaskCallbackInfo>(data.ReadParcelable<ContinuousTaskCallbackInfo>());
    if (!continuousTaskCallbackInfo) {
        BGTASK_LOGE("ContinuousTaskCallbackInfo ReadParcelable failed");
        return ERR_BGTASK_PARCELABLE_FAILED;
    }

    OnContinuousTaskStart(continuousTaskCallbackInfo);
    return ERR_OK;
}

ErrCode BackgroundTaskSubscriberStub::HandleOnContinuousTaskCancel(MessageParcel &data)
{
    BGTASK_LOGI("begin");
    std::shared_ptr<ContinuousTaskCallbackInfo> continuousTaskCallbackInfo
        = std::shared_ptr<ContinuousTaskCallbackInfo>(data.ReadParcelable<ContinuousTaskCallbackInfo>());
    if (!continuousTaskCallbackInfo) {
        BGTASK_LOGE("ContinuousTaskCallbackInfo ReadParcelable failed");
        return ERR_BGTASK_PARCELABLE_FAILED;
    }

    OnContinuousTaskStop(continuousTaskCallbackInfo);
    return ERR_OK;
}
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
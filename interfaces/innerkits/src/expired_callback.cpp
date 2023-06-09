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

#include "expired_callback.h"

namespace OHOS {
namespace BackgroundTaskMgr {
ExpiredCallback::ExpiredCallback()
{
    impl_ = new (std::nothrow) ExpiredCallbackImpl(*this);
};

ExpiredCallback::~ExpiredCallback() {}

const sptr<ExpiredCallback::ExpiredCallbackImpl> ExpiredCallback::GetImpl() const
{
    return impl_;
}

ExpiredCallback::ExpiredCallbackImpl::ExpiredCallbackImpl(ExpiredCallback &callback) : callback_(callback) {}

void ExpiredCallback::ExpiredCallbackImpl::OnExpired()
{
    callback_.OnExpired();
}
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
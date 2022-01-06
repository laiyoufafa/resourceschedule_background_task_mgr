/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_FRAMEWORKS_IPC_INCLUDE_IBACKGROUND_TASK_MGR_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_FRAMEWORKS_IPC_INCLUDE_IBACKGROUND_TASK_MGR_H

#include <string>

#include <ipc_types.h>
#include <iremote_broker.h>
#include <nocopyable.h>

#include "bgtaskmgr_inner_errors.h"
#include "delay_suspend_info.h"
#include "iexpired_callback.h"
#include "ibackground_task_subscriber.h"

namespace OHOS {
namespace BackgroundTaskMgr {
class IBackgroundTaskMgr : public IRemoteBroker {
public:
    IBackgroundTaskMgr() = default;
    ~IBackgroundTaskMgr() override = default;
    DISALLOW_COPY_AND_MOVE(IBackgroundTaskMgr);

    virtual ErrCode RequestSuspendDelay(const std::u16string& reason, 
        const sptr<IExpiredCallback>& callback, std::shared_ptr<DelaySuspendInfo> &delayInfo) = 0;
    virtual ErrCode  CancelSuspendDelay(int32_t requestId) = 0;
    virtual ErrCode GetRemainingDelayTime(int32_t requestId, int32_t &delayTime) = 0;
    virtual ErrCode  SubscribeBackgroundTask(const sptr<IBackgroundTaskSubscriber>& subscriber) = 0;
    virtual ErrCode  UnsubscribeBackgroundTask(const sptr<IBackgroundTaskSubscriber>& subscriber) = 0;
    virtual ErrCode  ShellDump(const std::vector<std::string> &dumpOption, std::vector<std::string> &dumpInfo) = 0;

public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.resourceschedule.IBackgroundTaskMgr");

protected:
    enum InterfaceId : uint32_t {
        REQUEST_SUSPEND_DELAY = MIN_TRANSACTION_ID,
        CANCEL_SUSPEND_DELAY,
        GET_REMAINING_DELAY_TIME,
        SUBSCRIBE_BACKGROUND_TASK,
        UNSUBSCRIBE_BACKGROUND_TASK,
        SHELL_DUMP,
    };
};
}
}
#endif // RESOURCESCHEDULE_BGTASKMGR_IBACKGROUND_TASK_MGR_H
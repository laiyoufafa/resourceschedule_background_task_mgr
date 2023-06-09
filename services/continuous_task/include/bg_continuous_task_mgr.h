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

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_SERVICES_CONTINUOUS_TASK_INCLUDE_BG_CONTINUOUS_TASK_MGR_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_SERVICES_CONTINUOUS_TASK_INCLUDE_BG_CONTINUOUS_TASK_MGR_H

#include <memory>
#include <mutex>

#include "ipc_skeleton.h"
#include "iremote_object.h"
#include "resource_manager.h"
#include "singleton.h"

#include "app_state_observer.h"
#include "bgtaskmgr_inner_errors.h"
#include "bundle_info.h"
#include "continuous_task_callback_info.h"
#include "data_storage.h"
#include "task_notification_subscriber.h"
#include "continuous_task_param.h"
#include "continuous_task_record.h"
#include "ibackground_task_subscriber.h"
#include "remote_death_recipient.h"
#include "system_event_observer.h"

namespace OHOS {
namespace BackgroundTaskMgr {
class BackgroundTaskMgrService;

enum class ContinuousTaskEventTriggerType: uint32_t {
    TASK_START,
    TASK_CANCEL,
};

struct CachedBundleInfo {
    std::unordered_map<std::string, uint32_t> abilityBgMode_ {};
    std::string appName_ {""};
};

class BgContinuousTaskMgr : public DelayedSingleton<BgContinuousTaskMgr>,
                            public std::enable_shared_from_this<BgContinuousTaskMgr> {
public:
    ErrCode StartBackgroundRunning(const sptr<ContinuousTaskParam> &taskParam);
    ErrCode StopBackgroundRunning(const std::string &abilityName);
    ErrCode AddSubscriber(const sptr<IBackgroundTaskSubscriber> &subscriber);
    ErrCode RemoveSubscriber(const sptr<IBackgroundTaskSubscriber> &subscriber);
    ErrCode ShellDump(const std::vector<std::string> &dumpOption, std::vector<std::string> &dumpInfo);
    bool StopContinuousTaskByUser(const std::string &mapKey);
    void OnAccountsStateChanged(int32_t id);
    void OnBundleInfoChanged(const std::string &action, const std::string &bundleName, int32_t uid);
    void OnAbilityStateChanged(int32_t uid, const std::string &abilityName);
    void OnProcessDied(int32_t pid);
    void OnRemoteSubscriberDied(const wptr<IRemoteObject> &object);
    bool Init();
    void InitNecessaryState();
    void InitRequiredResourceInfo();
    void Clear();

private:
    ErrCode StartBackgroundRunningInner(std::shared_ptr<ContinuousTaskRecord> &continuousTaskRecordPtr);
    ErrCode StopBackgroundRunningInner(int32_t uid, const std::string &abilityName);
    ErrCode AddSubscriberInner(const sptr<IBackgroundTaskSubscriber> &subscriber);
    ErrCode RemoveSubscriberInner(const sptr<IBackgroundTaskSubscriber> &subscriber);
    ErrCode ShellDumpInner(const std::vector<std::string> &dumpOption, std::vector<std::string> &dumpInfo);
    ErrCode SendContinuousTaskNotification(std::shared_ptr<ContinuousTaskRecord> &ContinuousTaskRecordPtr);
    void HandlePersistenceData();
    void CheckPersistenceData(const std::vector<AppExecFwk::RunningProcessInfo> &allProcesses,
        const std::vector<sptr<Notification::Notification>> &allNotifications);
    void DumpAllTaskInfo(std::vector<std::string> &dumpInfo);
    void DumpCancelTask(const std::vector<std::string> &dumpOption, bool cleanAll);
    bool RemoveContinuousTaskRecord(const std::string &mapKey);
    bool AddAppNameInfos(const AppExecFwk::BundleInfo &bundleInfo, CachedBundleInfo &cachedBundleInfo);
    std::string CreateNotificationLabel(int32_t uid, const std::string &bundleName,
        const std::string &abilityName);
    uint32_t GetBackgroundModeInfo(int32_t uid, std::string &abilityName);
    bool AddAbilityBgModeInfos(const AppExecFwk::BundleInfo &bundleInfo, CachedBundleInfo &cachedBundleInfo);
    bool RegisterNotificationSubscriber();
    bool RegisterSysCommEventListener();
    bool RegisterAppStateObserver();
    bool GetNotificationPrompt();
    bool SetCachedBundleInfo(int32_t uid, int32_t userId, std::string &bundleName);
    void OnRemoteSubscriberDiedInner(const wptr<IRemoteObject> &object);
    void OnContinuousTaskChanged(const std::shared_ptr<ContinuousTaskRecord> continuousTaskInfo,
        ContinuousTaskEventTriggerType changeEventType);
    bool checkBgmodeType(uint32_t configuredBgMode, uint32_t requestedBgModeId, bool isNewApi, int32_t uid);
    int32_t RefreshTaskRecord();

private:
    std::atomic<bool> isSysReady_ {false};
    std::string deviceType_ {""};
    std::shared_ptr<AppExecFwk::EventRunner> runner_ {nullptr};
    std::shared_ptr<AppExecFwk::EventHandler> handler_ {nullptr};
    std::unordered_map<std::string, std::shared_ptr<ContinuousTaskRecord>> continuousTaskInfosMap_ {};
    std::shared_ptr<TaskNotificationSubscriber> subscriber_ {nullptr};
    std::shared_ptr<SystemEventObserver> systemEventListener_ {nullptr};
    std::shared_ptr<AppStateObserver> appStateObserver_ {nullptr};
    std::list<sptr<IBackgroundTaskSubscriber>> bgTaskSubscribers_ {};
    std::map<sptr<IRemoteObject>, sptr<RemoteDeathRecipient>> subscriberRecipients_ {};
    std::unordered_map<int32_t, CachedBundleInfo> cachedBundleInfos_ {};
    std::vector<std::string> continuousTaskText_ {};
    std::shared_ptr<DataStorage> dataStorage_ {nullptr};

    DECLARE_DELAYED_SINGLETON(BgContinuousTaskMgr);
};
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
#endif  // FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_SERVICES_CONTINUOUS_TASK_INCLUDE_BG_CONTINUOUS_TASK_MGR_H
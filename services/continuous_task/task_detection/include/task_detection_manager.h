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

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_CONTINUOUS_TASK_TASK_DETECTION_MANAGER_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_CONTINUOUS_TASK_TASK_DETECTION_MANAGER_H

#include "hisysevent_manager.h"
#include "hisysevent_subscribe_callback.h"
#include "event_handler.h"
#include "event_runner.h"
#include "nlohmann/json.hpp"
#include "singleton.h"
#include "audio_stream_manager.h"
#include "common_utils.h"
#include "data_storage.h"
#include "event_type.h"
#include "running_process_info.h"
#ifdef AV_SESSION_PART_ENABLE
#include "avsession_info.h"
#include "avsession_manager.h"
#endif // AV_SESSION_PART_ENABLE

#ifdef BLUETOOTH_PART_ENABLE
#include "bluetooth_host.h"
#endif // BLUETOOTH_PART_ENABLE

#include "audio_detect.h"
#include "bluetooth_detect.h"
#include "location_detect.h"
#include "multi_device_detect.h"
#include "system_ability_status_change_stub.h"

namespace OHOS {
namespace BackgroundTaskMgr {
class TaskDetectionManager : public DelayedSingleton<TaskDetectionManager> {
public:
    bool Init(const std::shared_ptr<DataStorage> &dataStorage,
        const std::shared_ptr<AppExecFwk::EventHandler> &handler);
    bool CheckTaskRunningState(int32_t uid, uint32_t taskType);
    void HandleDisComponentChange(const std::string &info);
    void HandleProcessDied(int32_t uid, int32_t pid);
    void Dump(std::vector<std::string> &dumpInfo);
    void HandlePersistenceData(const std::vector<AppExecFwk::RunningProcessInfo> &allProcesses);
    void ReportNeedRecheckTask(int32_t uid, uint32_t taskType);
    void ReportStateChangeEvent(const EventType type, const std::string &infos);

private:
    bool AddSystemAbilityListener();
    bool InitHiSysEventListener();
    bool InitDisCompChangeObserver();
    bool InitAudioStateChangeListener();
    bool InitAVSessionStateChangeListener();
    bool InitBluetoothStateChangeObserver();
    void OnAddSystemAbility(int32_t systemAbilityId);
    void HandleSystemAbilityAdded(int32_t systemAbilityId);
    void OnRemoveSystemAbility(int32_t systemAbilityId);
    void HandleSystemAbilityRemoved(int32_t systemAbilityId);
    void ReportBluetoothPairState(const std::string &addr, int32_t state);
    void HandleBluetoothSysEvent(const nlohmann::json &root);
    void HandleLocationSysEvent(const nlohmann::json &root);
    void HandleAudioStreamInfo(const std::list<std::tuple<int32_t, int32_t, int32_t>> &streamInfos,
        const std::string &type);
#ifdef AV_SESSION_PART_ENABLE
    void HandleAVSessionInfo(const AVSession::AVSessionDescriptor &descriptor, const std::string &action);
#endif // AV_SESSION_PART_ENABLE
    bool CheckAudioCondition(int32_t uid, uint32_t taskType);
    bool CheckLocationCondition(int32_t uid);
    void ClearAllData();
    std::string ParseRecordToStr();
    bool ParseRecordFromJson(const nlohmann::json &value, std::set<int32_t> &uidSet);

private:
    class HiSysEventListener : public HiviewDFX::HiSysEventSubscribeCallBack {
    public:
        explicit HiSysEventListener();
        ~HiSysEventListener();
        void OnHandle(const std::string& domain, const std::string& eventName, const int32_t eventType,
            const std::string& eventDetail);
        void OnServiceDied();
    };

    class AudioRendererStateChangeListener : public AudioStandard::AudioRendererStateChangeCallback {
    public:
        void OnRendererStateChange(
        const std::vector<std::unique_ptr<AudioStandard::AudioRendererChangeInfo>> &audioRendererChangeInfos) override;
    };

    class AudioCapturerStateChangeListener : public AudioStandard::AudioCapturerStateChangeCallback {
    public:
        void OnCapturerStateChange(
        const std::vector<std::unique_ptr<AudioStandard::AudioCapturerChangeInfo>> &audioCapturerChangeInfos) override;
    };

    class SystemAbilityListener : public SystemAbilityStatusChangeStub {
    public:
        void OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
        void OnRemoveSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
    };

#ifdef AV_SESSION_PART_ENABLE
    class SessionStateListener : public AVSession::SessionListener {
    public:
        void OnSessionCreate(const AVSession::AVSessionDescriptor &descriptor) override;
        void OnSessionRelease(const AVSession::AVSessionDescriptor &descriptor) override;
        void OnTopSessionChange(const AVSession::AVSessionDescriptor &descriptor) override;
    };
#endif // AV_SESSION_PART_ENABLE

#ifdef BLUETOOTH_PART_ENABLE
    class BluetoothRemoteDeviceObserver : public Bluetooth::BluetoothRemoteDeviceObserver {
    public:
        void OnPairStatusChanged(const Bluetooth::BluetoothRemoteDevice &device, int status) override;
        void OnRemoteUuidChanged(const Bluetooth::BluetoothRemoteDevice &device,
            const std::vector<Bluetooth::ParcelUuid> &uuids) override;
        void OnRemoteNameChanged(const Bluetooth::BluetoothRemoteDevice &device,
            const std::string &deviceName) override;
        void OnRemoteAliasChanged(const Bluetooth::BluetoothRemoteDevice &device, const std::string &alias) override;
        void OnRemoteCodChanged(const Bluetooth::BluetoothRemoteDevice &device,
            const Bluetooth::BluetoothDeviceClass &cod) override;
        void OnRemoteBatteryLevelChanged(const Bluetooth::BluetoothRemoteDevice &device, int batteryLevel) override;
        void OnReadRemoteRssiEvent(const Bluetooth::BluetoothRemoteDevice &device, int rssi, int status) override;
    };
#endif // BLUETOOTH_PART_ENABLE

private:
    std::shared_ptr<HiSysEventListener> hiEventListener_ {nullptr};
    std::shared_ptr<AudioRendererStateChangeListener> playerListener_ {nullptr};
    std::shared_ptr<AudioCapturerStateChangeListener> recorderListener_ {nullptr};
#ifdef AV_SESSION_PART_ENABLE
    std::shared_ptr<SessionStateListener> avSessionStateListener_ {nullptr};
#endif // AV_SESSION_PART_ENABLE
    std::shared_ptr<DataStorage> dataStorage_ {nullptr};
    std::shared_ptr<AppExecFwk::EventHandler> handler_ {nullptr};
#ifdef BLUETOOTH_PART_ENABLE
    std::shared_ptr<BluetoothRemoteDeviceObserver> btRemoteDeviceObserver_ {nullptr};
#endif // BLUETOOTH_PART_ENABLE
    sptr<ISystemAbilityStatusChange> statusChangeListener_ {nullptr};
    std::shared_ptr<AudioDetect> audioDetect_ {nullptr};
    std::shared_ptr<BluetoothDetect> bluetoothDetect_ {nullptr};
    std::shared_ptr<LocationDetect> locationDetect_ {nullptr};
    std::shared_ptr<MultiDeviceDetect> multiDeviceDetect_ {nullptr};

    DECLARE_DELAYED_SINGLETON(TaskDetectionManager)
};
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
#endif  // FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_CONTINUOUS_TASK_TASK_DETECTION_MANAGER_H
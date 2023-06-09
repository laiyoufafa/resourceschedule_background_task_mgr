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

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_BGTASKMGR_SHELL_COMMAND_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_BGTASKMGR_SHELL_COMMAND_H

#include "shell_command.h"

#include "background_task_manager.h"

namespace OHOS {
namespace BackgroundTaskMgr {
class BgtaskmgrShellCommand : public ShellCommand {
public:
    BgtaskmgrShellCommand(int32_t argc, char *argv[]);
    ~BgtaskmgrShellCommand() override {};

private:
    ErrCode CreateCommandMap() override;
    ErrCode CreateMessageMap() override;
    ErrCode init() override;
    ErrCode RunAsHelpCommand();
    ErrCode RunAsDumpCommand();

private:
    std::shared_ptr<BackgroundTaskManager> btm_;
};
}  // namespace BackgroundTaskMgr
}  // namespace OHOS
#endif  // FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_BGTASKMGR_SHELL_COMMAND_H
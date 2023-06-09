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

#ifndef FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_SHELL_COMMAND_H
#define FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_SHELL_COMMAND_H

#include <map>
#include <string>
#include <functional>
#include <vector>

#include "errors.h"

namespace OHOS {
namespace BackgroundTaskMgr {
class ShellCommand {
public:
    ShellCommand(int32_t argc, char *argv[], std::string name);
    virtual ~ShellCommand();

    ErrCode OnCommand();
    std::string ExecCommand();
    std::string GetCommandErrorMsg() const;
    std::string GetUnknownOptionMsg(std::string &unknownOption) const;
    std::string GetMessageFromCode(const int32_t code) const;

    virtual ErrCode CreateCommandMap() = 0;
    virtual ErrCode CreateMessageMap() = 0;
    virtual ErrCode init() = 0;

protected:
    static constexpr uint32_t MIN_ARGUMENT_NUMBER = 2;

    int32_t argc_;
    char **argv_;

    std::string cmd_;
    std::vector<std::string> argList_;

    std::string name_;
    std::map<std::string, std::function<int32_t()>> commandMap_;
    std::map<int32_t, std::string> messageMap_;

    std::string resultReceiver_ = "";
};
}  // namespace BackgroundTaskMgr
}  // namespace OHOS

#endif  // FOUNDATION_RESOURCESCHEDULE_BACKGROUND_TASK_MGR_UTILS_DUMP_INCLUDE_SHELL_COMMAND_H
#pragma once

#include "ll/api/io/LogLevel.h"
#include <string>

namespace Catalyst {} // namespace Catalyst


struct Config {
    int              version    = 1;
    ll::io::LogLevel logLevel   = ll::io::LogLevel::Info; // 日志等级
    std::string      language   = "zh_CN";                // 语言设置
    bool             enableTest = false;                  // 是否开启测试监听
};

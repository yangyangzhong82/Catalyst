#pragma once

#include "ll/api/io/LogLevel.h"
#include <string>

namespace my_mod {} // namespace my_mod


struct Config {
    int              version  = 1;
    ll::io::LogLevel logLevel = ll::io::LogLevel::Info; // 日志等级
    std::string      language = "zh_CN";                // 语言设置
};

#pragma once
#include "ll/api/io/Logger.h"
#include "I18n/I18n.h"
#include "Config/Config.h"
struct Config;

namespace Catalyst {
extern ll::io::Logger& logger;
extern Config&         config;

} // namespace Catalyst

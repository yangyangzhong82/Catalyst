#include "mod/Entry.h"

#include "Config/ConfigManager.h"
#include "I18n/I18n.h"
#include "event/actor/player/PlayerEditSignEvent.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "ll/api/mod/RegisterHelper.h"


namespace Catalyst {
ll::io::Logger& logger = Entry::getInstance().getSelf().getLogger();
Config&         config = ConfigManager::getInstance().get();

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() {
    getSelf().getLogger().debug("Loading...");

    // 先初始化 I18n（使用默认语言）
    auto langPath = getSelf().getLangDir();
    I18n::getInstance().load(langPath.string(), "zh_CN");

    auto configPath = getSelf().getConfigDir();
    if (!std::filesystem::exists(configPath)) {
        std::filesystem::create_directories(configPath);
    }
    configPath /= "config.json";
    configPath.make_preferred();

    if (!ConfigManager::getInstance().load(configPath.string())) {
        getSelf().getLogger().error(tr("config.save_error", "load failed"));
        return false;
    }

    // 根据配置更新语言
    I18n::getInstance().setLanguage(ConfigManager::getInstance().get().language);

    logger.info(tr("plugin.loaded"));
    return true;
}

bool Entry::enable() {
    getSelf().getLogger().debug("Enabling...");

    // 测试监听 PlayerEditSignEvent
    auto& bus = ll::event::EventBus::getInstance();
    bus.emplaceListener<PlayerEditSignEvent>([](PlayerEditSignEvent& event) {
        logger.info(
            "PlayerEditSignEvent: player={}, pos=({},{},{}), oldFront='{}', newFront='{}'",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.oldFrontText(),
            event.newFrontText()
        );
    });

    return true;
}

bool Entry::disable() {
    getSelf().getLogger().debug("Disabling...");
    logger.info(tr("plugin.unloaded"));
    return true;
}


} // namespace Catalyst

LL_REGISTER_MOD(Catalyst::Entry, Catalyst::Entry::getInstance());

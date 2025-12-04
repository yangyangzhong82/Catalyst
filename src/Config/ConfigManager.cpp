#include "ConfigManager.h"
#include "ConfigSerialization.h"
#include "mod/Gloabl.h"
#include "config.h"
#include "mod/Entry.h"
#include "I18n/I18n.h"
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <vector>

namespace my_mod {

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() : mConfig(std::make_unique<Config>()) {}

ConfigManager::~ConfigManager() = default;

bool ConfigManager::load(const std::string& path) {
    mConfigPath = path;
    std::ifstream file(path);
    if (!file.is_open()) {
        // 文件不存在，创建默认配置
        logger.info(tr("config.not_found", path));
        return save();
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    nlohmann::json user_json;
    try {
        // 解析用户配置，允许注释
        user_json = nlohmann::json::parse(content, nullptr, true, true);
        // 填充内存中的配置对象。缺失的键将从结构体定义中获取默认值。
        *mConfig = user_json.get<Config>();
    } catch (const nlohmann::json::exception& e) {
        logger.error(tr("config.parse_error", e.what()));
        return false;
    }

    // 检查缺失的键以警告用户。
    nlohmann::json default_json = Config{}; // 创建一个默认配置对象并转换为json

    // 递归函数查找缺失的键
    std::function<void(const nlohmann::json&, const nlohmann::json&, std::string, std::vector<std::string>&)>
        find_missing_keys =
            [&](const nlohmann::json& d, const nlohmann::json& u, std::string p, std::vector<std::string>& m) {
                for (const auto& item : d.items()) {
                    if (!u.contains(item.key())) {
                        m.push_back(p + item.key());
                    } else if (item.value().is_object() && u.at(item.key()).is_object()) {
                        find_missing_keys(item.value(), u.at(item.key()), p + item.key() + ".", m);
                    }
                }
            };

    std::vector<std::string> missing_keys;
    find_missing_keys(default_json, user_json, "", missing_keys);

    if (!missing_keys.empty()) {
        auto& logger = Entry::getInstance().getSelf().getLogger();
        logger.warn(tr("config.missing_keys"));
        logger.warn(tr("config.missing_keys_hint"));
        for (const auto& key : missing_keys) {
            logger.warn(tr("config.missing_key_item", key));
        }
        logger.warn(tr("config.regenerate_hint"));
    }
    auto& logger = Entry::getInstance().getSelf().getLogger();
    logger.setLevel(mConfig->logLevel);

    return true; // 不再保存，以保留用户文件的格式和注释
}

bool ConfigManager::save() {
    try {
        nlohmann::json json = *mConfig;
        std::ofstream  file(mConfigPath);
        file << json.dump(4);
    } catch (const nlohmann::json::exception& e) {
        Entry::getInstance().getSelf().getLogger().error(tr("config.save_error", e.what()));
        return false;
    }
    return true;
}



Config& ConfigManager::get() { return *mConfig; }

const Config& ConfigManager::get() const { return *mConfig; }

} // namespace CT

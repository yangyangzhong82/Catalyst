#include "I18n.h"
#include "catalyst/mod/Gloabl.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Catalyst {

I18n& I18n::getInstance() {
    static I18n instance;
    return instance;
}

bool I18n::load(const std::string& langDir, const std::string& defaultLang) {
    mDefaultLang = defaultLang;
    mCurrentLang = defaultLang;

    namespace fs = std::filesystem;
    if (!fs::exists(langDir)) {
        fs::create_directories(langDir);
        logger.warn(get("i18n.lang_dir_not_found", langDir));
        return false;
    }

    bool loaded = false;
    for (const auto& entry : fs::directory_iterator(langDir)) {
        if (entry.path().extension() == ".json") {
            std::string lang = entry.path().stem().string();
            if (loadLanguageFile(entry.path().string(), lang)) {
                loaded = true;
            }
        }
    }
    return loaded;
}

bool I18n::loadLanguageFile(const std::string& path, const std::string& lang) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    try {
        nlohmann::json json;
        file >> json;

        for (const auto& [key, value] : json.items()) {
            if (value.is_string()) {
                mTranslations[lang][key] = value.get<std::string>();
            }
        }
        logger.info(get("i18n.loaded", lang));
        return true;
    } catch (const std::exception& e) {
        logger.error(get("i18n.load_error", path, e.what()));
        return false;
    }
}

void I18n::setLanguage(const std::string& lang) {
    if (mTranslations.contains(lang)) {
        mCurrentLang = lang;
    } else {
        logger.warn(get("i18n.lang_not_found", lang, mDefaultLang));
        mCurrentLang = mDefaultLang;
    }
}

const std::string& I18n::getLanguage() const { return mCurrentLang; }

const std::string& I18n::get(const std::string& key) const {
    // 先查找当前语言
    if (auto langIt = mTranslations.find(mCurrentLang); langIt != mTranslations.end()) {
        if (auto keyIt = langIt->second.find(key); keyIt != langIt->second.end()) {
            return keyIt->second;
        }
    }
    // 回退到默认语言
    if (mCurrentLang != mDefaultLang) {
        if (auto langIt = mTranslations.find(mDefaultLang); langIt != mTranslations.end()) {
            if (auto keyIt = langIt->second.find(key); keyIt != langIt->second.end()) {
                return keyIt->second;
            }
        }
    }
    // 返回key本身
    return key;
}

} // namespace Catalyst

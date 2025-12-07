#pragma once

#include <string>
#include <unordered_map>
#include <fmt/format.h>

namespace Catalyst {

class I18n {
public:
    static I18n& getInstance();

    // 加载语言文件目录
    bool load(const std::string& langDir, const std::string& defaultLang = "zh_CN");

    // 设置当前语言
    void setLanguage(const std::string& lang);

    // 获取当前语言
    const std::string& getLanguage() const;

    // 获取翻译文本
    const std::string& get(const std::string& key) const;

    // 带格式化参数的翻译
    template <typename... Args>
    std::string get(const std::string& key, Args&&... args) const {
        return fmt::format(fmt::runtime(get(key)), std::forward<Args>(args)...);
    }

private:
    I18n() = default;
    ~I18n() = default;
    I18n(const I18n&) = delete;
    I18n& operator=(const I18n&) = delete;

    bool loadLanguageFile(const std::string& path, const std::string& lang);

    std::string mCurrentLang;
    std::string mDefaultLang;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mTranslations;
};

// 便捷函数
inline const std::string& tr(const std::string& key) {
    return I18n::getInstance().get(key);
}

template <typename... Args>
inline std::string tr(const std::string& key, Args&&... args) {
    return I18n::getInstance().get(key, std::forward<Args>(args)...);
}

} // namespace Catalyst

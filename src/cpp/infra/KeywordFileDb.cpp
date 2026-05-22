#include "infra/KeywordFileDb.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace {

std::string readFileUtf8(const std::string& path) {
    std::ifstream file(std::filesystem::path(path), std::ios::binary);
    if (!file) {
        return {};
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    if (content.size() >= 3 && static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF) {
        content.erase(0, 3);
    }
    return content;
}

bool parseKeywordJson(const nlohmann::json& root, KeywordData& out) {
    if (!root.is_object()) {
        return false;
    }

    const auto sentimentIt = root.find("sentiment");
    const auto categoryIt = root.find("category");
    if (sentimentIt == root.end() || categoryIt == root.end()) {
        return false;
    }
    if (!sentimentIt->is_object() || !categoryIt->is_object()) {
        return false;
    }

    out.sentiment.clear();
    for (const auto& [key, value] : sentimentIt->items()) {
        if (!value.is_array()) {
            return false;
        }
        std::vector<std::string> keywords;
        for (const auto& item : value) {
            if (!item.is_string()) {
                return false;
            }
            keywords.push_back(item.template get<std::string>());
        }
        out.sentiment.emplace(key, std::move(keywords));
    }

    out.category.clear();
    for (const auto& [catKey, catValue] : categoryIt->items()) {
        if (!catValue.is_object()) {
            return false;
        }
        std::map<std::string, std::vector<std::string>> subMap;
        for (const auto& [subKey, subValue] : catValue.items()) {
            if (!subValue.is_array()) {
                return false;
            }
            std::vector<std::string> keywords;
            for (const auto& item : subValue) {
                if (!item.is_string()) {
                    return false;
                }
                keywords.push_back(item.template get<std::string>());
            }
            subMap.emplace(subKey, std::move(keywords));
        }
        out.category.emplace(catKey, std::move(subMap));
    }

    return true;
}

}  // namespace

bool KeywordFileDb::loadFromString(const std::string& json, KeywordData& out) {
    out.sentiment.clear();
    out.category.clear();
    if (json.empty()) {
        return false;
    }
    try {
        const auto root = nlohmann::json::parse(json);
        return parseKeywordJson(root, out);
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool KeywordFileDb::load(const std::string& path, KeywordData& out) {
    const std::string content = readFileUtf8(path);
    if (content.empty()) {
        return false;
    }
    return loadFromString(content, out);
}

bool KeywordFileDb::validate(const KeywordData& data) {
    return data.sentiment.count(u8"긍정") > 0 && data.sentiment.count(u8"부정") > 0 &&
           !data.category.empty();
}

#pragma once

#include <map>
#include <string>
#include <vector>

struct KeywordData {
    std::map<std::string, std::vector<std::string>> sentiment;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> category;
};

class KeywordFileDb {
public:
    /** Load keywords from JSON file. Returns false if file missing or parse invalid. */
    static bool load(const std::string& path, KeywordData& out);

    /** Parse keywords from JSON string (tests and tooling). */
    static bool loadFromString(const std::string& json, KeywordData& out);

    /** True if loaded data has minimum required sentiment keys. */
    static bool validate(const KeywordData& data);
};

#pragma once

#include "services/TrendTypes.h"

#include <string>
#include <vector>

class TrendLoader {
public:
    static bool load(const std::string& path, std::vector<TrendRecord>& out);
    static bool loadFromContent(const std::string& csvContent, std::vector<TrendRecord>& out);
};

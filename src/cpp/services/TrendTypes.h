#pragma once

#include <string>
#include <vector>

struct TrendDayRow {
    std::string date;
    int positive{0};
    int neutral{0};
    int negative{0};
    int total{0};
};

struct TrendSnapshot {
    std::vector<TrendDayRow> byDay;
    bool empty() const { return byDay.empty(); }
};

struct TrendRecord {
    std::string date;
    std::string text;
    std::string sentimentOverride;
};

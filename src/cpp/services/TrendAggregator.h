#pragma once

#include "services/TrendTypes.h"

#include <vector>

class TrendAggregator {
public:
    static TrendSnapshot buildSnapshot(const std::vector<TrendRecord>& records);
};

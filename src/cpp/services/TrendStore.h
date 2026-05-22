#pragma once

#include "services/TrendTypes.h"

class TrendStore {
public:
    static void loadAtStartup();
    static const TrendSnapshot& getSnapshot();
};

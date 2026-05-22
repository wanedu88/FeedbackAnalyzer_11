#include "services/TrendStore.h"

#include "AppConfig.h"
#include "services/TrendAggregator.h"
#include "services/TrendLoader.h"

#include <vector>

#ifdef PROJECT_ROOT
#include <string>
#endif

namespace {

TrendSnapshot gSnapshot;

std::vector<std::string> trendSearchPaths() {
    std::vector<std::string> paths;
#ifdef PROJECT_ROOT
    paths.push_back(std::string(PROJECT_ROOT) + "/data/test_feedback_trend.csv");
#endif
    paths.emplace_back(AppConfig::kTrendCsvRelative);
    paths.emplace_back(AppConfig::kTrendCsvRelativeParent);
    return paths;
}

}  // namespace

void TrendStore::loadAtStartup() {
    gSnapshot = TrendSnapshot{};
    std::vector<TrendRecord> records;
    for (const auto& path : trendSearchPaths()) {
        if (TrendLoader::load(path, records)) {
            gSnapshot = TrendAggregator::buildSnapshot(records);
            return;
        }
    }
}

const TrendSnapshot& TrendStore::getSnapshot() {
    return gSnapshot;
}

#include "services/TrendAggregator.h"

#include "SentimentClassifier.h"

#include <map>

namespace {

std::string resolveSentiment(const TrendRecord& record) {
    if (!record.sentimentOverride.empty()) {
        return record.sentimentOverride;
    }
    return SentimentClassifier::classifySentiment(record.text);
}

}  // namespace

TrendSnapshot TrendAggregator::buildSnapshot(const std::vector<TrendRecord>& records) {
    TrendSnapshot snapshot;
    std::map<std::string, TrendDayRow> byDate;

    for (const auto& record : records) {
        auto& row = byDate[record.date];
        if (row.date.empty()) {
            row.date = record.date;
        }
        const std::string sentiment = resolveSentiment(record);
        if (sentiment == u8"긍정") {
            ++row.positive;
        } else if (sentiment == u8"부정") {
            ++row.negative;
        } else {
            ++row.neutral;
        }
        ++row.total;
    }

    for (const auto& entry : byDate) {
        snapshot.byDay.push_back(entry.second);
    }
    return snapshot;
}

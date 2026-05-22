#include <gtest/gtest.h>

#include "Constants.h"
#include "services/TrendAggregator.h"
#include "services/TrendLoader.h"

#include <string>

namespace {

std::string fixturePath(const char* name) {
#ifdef PROJECT_ROOT
    return std::string(PROJECT_ROOT) + "/tests/fixtures/" + name;
#else
    return std::string("../tests/fixtures/") + name;
#endif
}

}  // namespace

class TrendLoaderTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { Constants::init(); }
};

TEST_F(TrendLoaderTest, LoadFromContent_ParsesValidRows) {
    const std::string csv =
        "date,text,sentiment\n"
        "2025-05-01," + std::string(u8"최고입니다") + ",긍정\n"
        "2025-05-02," + std::string(u8"환불 요청") + ",부정\n";

    std::vector<TrendRecord> records;
    ASSERT_TRUE(TrendLoader::loadFromContent(csv, records));
    ASSERT_EQ(records.size(), 2u);
    EXPECT_EQ(records[0].date, "2025-05-01");
    EXPECT_EQ(records[1].sentimentOverride, u8"부정");
}

TEST_F(TrendLoaderTest, LoadFromContent_SkipsInvalidDateRows) {
    const std::string csv =
        "date,text\n"
        "not-a-date," + std::string(u8"무시") + "\n"
        "2025-05-03," + std::string(u8"정상") + "\n";

    std::vector<TrendRecord> records;
    ASSERT_TRUE(TrendLoader::loadFromContent(csv, records));
    ASSERT_EQ(records.size(), 1u);
    EXPECT_EQ(records[0].date, "2025-05-03");
}

TEST_F(TrendLoaderTest, LoadFixtureFile_Succeeds) {
    std::vector<TrendRecord> records;
    ASSERT_TRUE(TrendLoader::load(fixturePath("trend_sample.csv"), records));
    EXPECT_GE(records.size(), 2u);
}

TEST_F(TrendLoaderTest, Aggregate_ByDay_CountsSentiments) {
    const std::string csv =
        "date,text,sentiment\n"
        "2025-05-01," + std::string(u8"최고") + ",긍정\n"
        "2025-05-01," + std::string(u8"별로") + ",부정\n"
        "2025-05-02," + std::string(u8"그냥") + ",\n";

    std::vector<TrendRecord> records;
    ASSERT_TRUE(TrendLoader::loadFromContent(csv, records));
    const TrendSnapshot snapshot = TrendAggregator::buildSnapshot(records);

    ASSERT_EQ(snapshot.byDay.size(), 2u);

    int day1Total = 0;
    int day1Positive = 0;
    for (const auto& row : snapshot.byDay) {
        if (row.date == "2025-05-01") {
            day1Total = row.total;
            day1Positive = row.positive;
        }
    }
    EXPECT_EQ(day1Total, 2);
    EXPECT_EQ(day1Positive, 1);
}

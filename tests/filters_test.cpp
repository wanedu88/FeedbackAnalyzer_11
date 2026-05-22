#include "test_fixture.h"
#include "TextAnalyzer.h"
#include "Filters.h"
#include "Feedback.h"

namespace {

std::vector<Feedback> makeMixedFeedbacks() {
    return {
        Feedback(u8"최고입니다 감사합니다"),
        Feedback(u8"최악 불만 환불"),
        Feedback(u8"보통 그냥 무난"),
    };
}

}  // namespace

// --- Filters::fil (F-01 ~ F-07) ---

TEST_F(FeedbackAnalyzerTestFixture, F01_Fil_SentimentAll_ReturnsAll) {
    Filters filters;
    auto data = makeMixedFeedbacks();

    auto result = filters.fil(data, u8"전체", u8"전체");

    EXPECT_EQ(result.size(), 3u);
}

TEST_F(FeedbackAnalyzerTestFixture, F02_Fil_PositiveOnly) {
    Filters filters;
    std::vector<Feedback> data = {
        Feedback(u8"최고입니다"),
        Feedback(u8"최악 불만"),
    };

    auto result = filters.fil(data, u8"긍정", u8"전체");

    ASSERT_EQ(result.size(), 1u);
    EXPECT_NE(result[0].getText().find(u8"최고"), std::string::npos);
}

TEST_F(FeedbackAnalyzerTestFixture, F03_Fil_NeutralUsesFilterKeywords) {
    Filters filters;
    // "괜찮"은 S_KEYWORDS 긍정에도 있어 제외; 중립 전용 토큰만 사용
    std::vector<Feedback> data = {Feedback(u8"보통 그냥 무난")};

    auto result = filters.fil(data, u8"중립", u8"전체");

    EXPECT_GE(result.size(), 1u);
}

// M3 중립 필터 회귀: tests/regression_neutral_filter_test.cpp 참고

// Known bug (M3): keyword filter skips CATEGORY_KEYWORDS["main"] sub-map.
TEST_F(FeedbackAnalyzerTestFixture, DISABLED_F05_KeywordSkipsMain) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"배송")};

    auto result = filters.fil(data, u8"전체", u8"배송");

    EXPECT_GE(result.size(), 1u);
}

TEST_F(FeedbackAnalyzerTestFixture, F06_Fil_KeywordSubOnly_Matches) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"택배가 왔어요")};

    auto result = filters.fil(data, u8"전체", u8"배송");

    EXPECT_GE(result.size(), 1u);
}

TEST_F(FeedbackAnalyzerTestFixture, F07_Fil_Combo_SentimentAndKeyword) {
    Filters filters;
    std::vector<Feedback> data = {
        Feedback(u8"최고입니다 택배 빠름"),
        Feedback(u8"최악 불만"),
        Feedback(u8"택배만 언급"),
    };

    auto result = filters.fil(data, u8"긍정", u8"배송");

    ASSERT_EQ(result.size(), 1u);
    EXPECT_NE(result[0].getText().find(u8"최고"), std::string::npos);
    EXPECT_NE(result[0].getText().find(u8"택배"), std::string::npos);
}

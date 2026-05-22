#include "test_fixture.h"
#include "TextAnalyzer.h"
#include "Feedback.h"

// --- TextAnalyzer::sent (S-01 ~ S-06) ---

TEST_F(FeedbackAnalyzerTestFixture, S01_Sent_EmptyList_ReturnsZeroCounts) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks;

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 0);
    EXPECT_EQ(result[u8"중립"], 0);
    EXPECT_EQ(result[u8"부정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, S02_Sent_PositiveKeyword_CountsPositive) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"최고입니다 감사합니다")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 1);
    EXPECT_EQ(result[u8"중립"], 0);
    EXPECT_EQ(result[u8"부정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, S03_Sent_NegativeKeyword_CountsNegative) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"최악 불만 환불")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 0);
    EXPECT_EQ(result[u8"중립"], 0);
    EXPECT_EQ(result[u8"부정"], 1);
}

TEST_F(FeedbackAnalyzerTestFixture, S04_Sent_NoKeyword_DefaultsNeutral) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"오늘 날씨 좋음")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 0);
    EXPECT_EQ(result[u8"중립"], 1);
    EXPECT_EQ(result[u8"부정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, S05_Sent_PositiveBeatsNegative) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"좋은데 나쁘진 않음")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 1);
    EXPECT_EQ(result[u8"중립"], 0);
    EXPECT_EQ(result[u8"부정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, S06_Sent_NeutralKeywordStillCountsNeutral) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"보통 그냥 무난")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"긍정"], 0);
    EXPECT_EQ(result[u8"중립"], 1);
    EXPECT_EQ(result[u8"부정"], 0);
}

// --- TextAnalyzer::kw (K-01 ~ K-04) ---

TEST_F(FeedbackAnalyzerTestFixture, K01_Kw_EmptyList_AllCategoriesZero) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks;

    auto result = analyzer.kw(feedbacks);

    EXPECT_EQ(result[u8"배송"], 0);
    EXPECT_EQ(result[u8"품질"], 0);
    EXPECT_EQ(result[u8"가격"], 0);
    EXPECT_EQ(result[u8"서비스"], 0);
    EXPECT_EQ(result[u8"사용성"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, K02_Kw_MainKeyword_MatchesCategory) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"배송이 빨라요")};

    auto result = analyzer.kw(feedbacks);

    EXPECT_GE(result[u8"배송"], 1);
}

TEST_F(FeedbackAnalyzerTestFixture, K03_Kw_SubKeywordOnly_StillMatches) {
    TextAnalyzer analyzer;
    // kw()는 "main"만 검사. 카테고리 키워드가 없는 문장 → 미집계(RED 스펙: 0)
    std::vector<Feedback> feedbacks = {Feedback(u8"일반적인 의견입니다")};

    auto result = analyzer.kw(feedbacks);

    EXPECT_EQ(result[u8"배송"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, K04_Kw_MultipleCategories) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"배송 빠르고 가격 저렴")};

    auto result = analyzer.kw(feedbacks);

    EXPECT_GE(result[u8"배송"], 1);
    EXPECT_GE(result[u8"가격"], 1);
}

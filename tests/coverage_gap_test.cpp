#include "test_fixture.h"
#include "TextAnalyzer.h"
#include "Filters.h"
#include "Feedback.h"

// --- globalSent / globalKw assignment (lines 38, 61 in TextAnalyzer.h) ---

TEST_F(FeedbackAnalyzerTestFixture, COV_G01_GlobalSent_SecondCallOverwrites) {
    TextAnalyzer analyzer;
    std::vector<Feedback> first = {Feedback(u8"최고입니다")};
    std::vector<Feedback> second = {Feedback(u8"최악 불만")};

    auto r1 = analyzer.sent(first);
    auto r2 = analyzer.sent(second);

    EXPECT_EQ(r1[u8"긍정"], 1);
    EXPECT_EQ(r2[u8"부정"], 1);
    EXPECT_EQ(r2[u8"긍정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, COV_G02_GlobalKw_SecondCallOverwrites) {
    TextAnalyzer analyzer;
    std::vector<Feedback> first = {Feedback(u8"배송 빠름")};
    std::vector<Feedback> second = {Feedback(u8"가격 저렴")};

    auto r1 = analyzer.kw(first);
    auto r2 = analyzer.kw(second);

    EXPECT_GE(r1[u8"배송"], 1);
    EXPECT_EQ(r1[u8"가격"], 0);
    EXPECT_GE(r2[u8"가격"], 1);
    EXPECT_EQ(r2[u8"배송"], 0);
}

// --- TextAnalyzer::containsAny false return (line 17) ---

TEST_F(FeedbackAnalyzerTestFixture, COV_TA01_ContainsAny_NoMatch_ReturnsNeutral) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"키워드없는일반문장xyz")};

    auto result = analyzer.sent(feedbacks);

    EXPECT_EQ(result[u8"중립"], 1);
    EXPECT_EQ(result[u8"긍정"], 0);
    EXPECT_EQ(result[u8"부정"], 0);
}

TEST_F(FeedbackAnalyzerTestFixture, COV_TA02_Kw_ContainsAny_NoCategoryMatch) {
    TextAnalyzer analyzer;
    std::vector<Feedback> feedbacks = {Feedback(u8"카테고리무관")};

    auto result = analyzer.kw(feedbacks);

    EXPECT_EQ(result[u8"배송"], 0);
    EXPECT_EQ(result[u8"품질"], 0);
}

// --- Filters::containsAny + fil branches ---

TEST_F(FeedbackAnalyzerTestFixture, COV_F01_Fil_NegativeOnly) {
    Filters filters;
    std::vector<Feedback> data = {
        Feedback(u8"최고입니다"),
        Feedback(u8"최악 환불"),
    };

    auto result = filters.fil(data, u8"부정", u8"전체");

    ASSERT_EQ(result.size(), 1u);
    EXPECT_NE(result[0].getText().find(u8"최악"), std::string::npos);
}

TEST_F(FeedbackAnalyzerTestFixture, COV_F02_Fil_InvalidKeywordCategory) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"택배 빠름")};

    auto result = filters.fil(data, u8"전체", u8"없는카테고리");

    EXPECT_TRUE(result.empty());
}

TEST_F(FeedbackAnalyzerTestFixture, COV_F03_Fil_KeywordNoSubMatch_EmptyResult) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"최고입니다")};

    auto result = filters.fil(data, u8"전체", u8"배송");

    EXPECT_TRUE(result.empty());
}

TEST_F(FeedbackAnalyzerTestFixture, COV_F04_Fil_SentimentExcludesNonMatching) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"최악 불만")};

    auto result = filters.fil(data, u8"긍정", u8"전체");

    EXPECT_TRUE(result.empty());
}

TEST_F(FeedbackAnalyzerTestFixture, COV_F05_Fil_CoutLoopRunsOnNonEmptyResult) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"보통"), Feedback(u8"무난")};

    auto result = filters.fil(data, u8"중립", u8"전체");

    EXPECT_GE(result.size(), 1u);
}

TEST_F(FeedbackAnalyzerTestFixture, COV_F06_Fil_CoutLoopSkippedOnEmptyResult) {
    Filters filters;
    std::vector<Feedback> data = {Feedback(u8"최고입니다")};

    auto result = filters.fil(data, u8"부정", u8"전체");

    EXPECT_TRUE(result.empty());
}

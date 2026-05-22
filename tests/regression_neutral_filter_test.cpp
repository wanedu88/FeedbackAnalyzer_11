#include "test_fixture.h"
#include "TextAnalyzer.h"
#include "Filters.h"
#include "Feedback.h"

// Mission 3 regression: sent() 중립 집계 건수 == fil(중립) 결과 건수
// M3 GREEN: SentimentClassifier::classifySentiment shared by sent() and fil().

namespace {

int neutralCountFromSent(const std::vector<Feedback>& data) {
    TextAnalyzer analyzer;
    return analyzer.countSentiments(data).at(u8"중립");
}

size_t neutralCountFromFil(const std::vector<Feedback>& data) {
    Filters filters;
    return filters.filterFeedbacks(data, u8"중립", u8"전체").size();
}

void ExpectSentNeutralCountEqualsFilNeutralCount(const std::vector<Feedback>& data) {
    EXPECT_EQ(neutralCountFromSent(data), static_cast<int>(neutralCountFromFil(data)));
}

}  // namespace

// --- Known mismatch samples (Constants::sent vs Filters::S_KEYWORDS) ---

// Case 1: sent=중립(기본), fil=긍정(S_KEYWORDS "괜찮" 우선) → fil(중립) 0건
TEST_F(FeedbackAnalyzerTestFixture, Regression_NeutralFilterMismatch_Case1_Gwaenchan) {
    std::vector<Feedback> data = {Feedback(u8"괜찮해요")};
    ExpectSentNeutralCountEqualsFilNeutralCount(data);
}

// Case 2: 본문에 "괜찮" 포함 → sent 중립, fil 긍정 분류
TEST_F(FeedbackAnalyzerTestFixture, Regression_NeutralFilterMismatch_Case2_GwaenchanInSentence) {
    std::vector<Feedback> data = {Feedback(u8"괜찮한데 배송은 보통이에요")};
    ExpectSentNeutralCountEqualsFilNeutralCount(data);
}

// Case 3: 감정 키워드 없음 → 양쪽 모두 중립 (수정 후에도 통과해야 하는 대조 케이스)
TEST_F(FeedbackAnalyzerTestFixture, Regression_NeutralFilterMismatch_Case3_NoKeywordDefaultsNeutral) {
    std::vector<Feedback> data = {Feedback(u8"오늘 날씨 좋음")};
    ExpectSentNeutralCountEqualsFilNeutralCount(data);
}

// Mixed batch: sent 중립 3건 vs fil(중립) 2건 (현재 버그)
TEST_F(FeedbackAnalyzerTestFixture, Regression_NeutralFilterMismatch) {
    std::vector<Feedback> data = {
        Feedback(u8"괜찮해요"),
        Feedback(u8"오늘 날씨 좋음"),
        Feedback(u8"보통 그냥 무난"),
    };
    ExpectSentNeutralCountEqualsFilNeutralCount(data);
}

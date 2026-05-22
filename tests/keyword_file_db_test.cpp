#include <gtest/gtest.h>

#include "Constants.h"
#include "SentimentClassifier.h"
#include "infra/KeywordFileDb.h"

#include <algorithm>
#include <string>

namespace {

std::string fixturePath(const char* name) {
#ifdef PROJECT_ROOT
    return std::string(PROJECT_ROOT) + "/tests/fixtures/" + name;
#else
    return std::string("../tests/fixtures/") + name;
#endif
}

bool vectorContains(const std::vector<std::string>& values, const std::string& needle) {
    return std::find(values.begin(), values.end(), needle) != values.end();
}

}  // namespace

TEST(KeywordFileDbTest, LoadFromString_MinimalJson_PopulatesMaps) {
    const std::string json =
        std::string(u8"{\"version\":1,\"sentiment\":{\"")
        + u8"긍정" + u8"\":[\"" + u8"테스트긍정키워드" + u8"\"],\"" + u8"부정" + u8"\":[\"" +
        u8"테스트부정키워드" + u8"\"]},\"category\":{\"" + u8"배송" + u8"\":{\"main\":[\"" +
        u8"배송테스트" + u8"\"]}}}";
    KeywordData data;
    ASSERT_TRUE(KeywordFileDb::loadFromString(json, data));
    ASSERT_TRUE(KeywordFileDb::validate(data));
    EXPECT_TRUE(vectorContains(data.sentiment.at(u8"긍정"), u8"테스트긍정키워드"));
}

TEST(KeywordFileDbTest, LoadMinimalJson_PopulatesMaps) {
    KeywordData data;
    ASSERT_TRUE(KeywordFileDb::load(fixturePath("keywords_minimal.json"), data));
    ASSERT_TRUE(KeywordFileDb::validate(data));

    EXPECT_TRUE(vectorContains(data.sentiment.at(u8"긍정"), u8"테스트긍정키워드"));
    EXPECT_TRUE(vectorContains(data.sentiment.at(u8"부정"), u8"테스트부정키워드"));
    EXPECT_EQ(data.category.at(u8"배송").at(u8"main").at(0), u8"배송테스트");
}

TEST(KeywordFileDbTest, LoadMissingFile_ReturnsFalse) {
    KeywordData data;
    EXPECT_FALSE(KeywordFileDb::load("nonexistent_keywords_7.json", data));
}

TEST(KeywordFileDbTest, LoadInvalidJson_ReturnsFalse) {
    KeywordData data;
    EXPECT_FALSE(KeywordFileDb::load(fixturePath("keywords_invalid.json"), data));
}

TEST(KeywordFileDbTest, Validate_RejectsMissingSentimentKeys) {
    KeywordData data;
    data.category[u8"배송"][u8"main"] = {u8"배송"};
    EXPECT_FALSE(KeywordFileDb::validate(data));
}

TEST(KeywordFileDbTest, LoadProjectKeywords_MatchesRuntimeConstants) {
    const std::vector<std::string> candidates = {
#ifdef PROJECT_ROOT
        std::string(PROJECT_ROOT) + "/data/keywords.json",
#endif
        "../data/keywords.json",
        "data/keywords.json",
    };
    KeywordData data;
    bool loaded = false;
    for (const auto& path : candidates) {
        if (KeywordFileDb::load(path, data)) {
            loaded = true;
            break;
        }
    }
    ASSERT_TRUE(loaded);
    EXPECT_TRUE(vectorContains(data.sentiment.at(u8"긍정"), u8"최고입니다"));
    EXPECT_TRUE(vectorContains(data.sentiment.at(u8"부정"), u8"환불"));
    EXPECT_TRUE(data.category.count(u8"배송") > 0);
}

class ConstantsKeywordInitTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { Constants::init(); }
};

TEST_F(ConstantsKeywordInitTest, Init_LoadsKeywordsFromFileWhenPresent) {
    ASSERT_FALSE(Constants::SENTIMENT_KEYWORDS.empty());
    EXPECT_TRUE(vectorContains(Constants::SENTIMENT_KEYWORDS.at(u8"긍정"), u8"최고입니다"));
    EXPECT_TRUE(Constants::CATEGORY_KEYWORDS.count(u8"서비스") > 0);
}

TEST_F(ConstantsKeywordInitTest, ClassifySentiment_UsesLoadedKeywords) {
    EXPECT_EQ(SentimentClassifier::classifySentiment(u8"최고입니다 감사합니다"), u8"긍정");
    EXPECT_EQ(SentimentClassifier::classifySentiment(u8"환불 요청"), u8"부정");
    EXPECT_EQ(SentimentClassifier::classifySentiment(u8"오늘 날씨 좋음"), u8"중립");
}

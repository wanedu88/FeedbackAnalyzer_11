#include <gtest/gtest.h>
#include "ParseUtils.h"

// --- ParseUtils::urlDecode (U-01 ~ U-03) ---

TEST(ParseUtilsTest, U01_UrlDecode_PlusToSpace) {
    EXPECT_EQ(ParseUtils::urlDecode("hello+world"), "hello world");
}

TEST(ParseUtilsTest, U02_UrlDecode_PercentUtf8) {
    EXPECT_EQ(ParseUtils::urlDecode("%EB%82%A0"), u8"날");
}

TEST(ParseUtilsTest, U03_UrlDecode_InvalidPercent_KeepsChar) {
    EXPECT_EQ(ParseUtils::urlDecode("%GG"), "%GG");
}

// --- ParseUtils::parseCsvLine (C-01 ~ C-04) ---

TEST(ParseUtilsTest, C01_ParseCsv_SimpleComma) {
    auto fields = ParseUtils::parseCsvLine("a,b,c");

    ASSERT_EQ(fields.size(), 3u);
    EXPECT_EQ(fields[0], "a");
    EXPECT_EQ(fields[1], "b");
    EXPECT_EQ(fields[2], "c");
}

TEST(ParseUtilsTest, C02_ParseCsv_QuotedComma) {
    auto fields = ParseUtils::parseCsvLine("\"a,b\",c");

    ASSERT_EQ(fields.size(), 2u);
    EXPECT_EQ(fields[0], "a,b");
    EXPECT_EQ(fields[1], "c");
}

TEST(ParseUtilsTest, C03_ParseCsv_EmptyField) {
    auto fields = ParseUtils::parseCsvLine("a,,c");

    ASSERT_EQ(fields.size(), 3u);
    EXPECT_EQ(fields[0], "a");
    EXPECT_EQ(fields[1], "");
    EXPECT_EQ(fields[2], "c");
}

TEST(ParseUtilsTest, C04_ParseCsv_QuotesToggle) {
    auto fields = ParseUtils::parseCsvLine("\"\"\"a\"\"\"");

    ASSERT_EQ(fields.size(), 1u);
    EXPECT_EQ(fields[0], "a");
}

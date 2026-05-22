#pragma once

namespace AppConfig {
constexpr int kServerPort = 8080;
constexpr const char* kServerHost = "0.0.0.0";
constexpr int kTextareaHeightPx = 100;

/** Relative to process cwd (e.g. build/). */
constexpr const char* kKeywordsJsonRelative = "data/keywords.json";
constexpr const char* kKeywordsJsonRelativeParent = "../data/keywords.json";
constexpr const char* kTrendCsvRelative = "data/test_feedback_trend.csv";
constexpr const char* kTrendCsvRelativeParent = "../data/test_feedback_trend.csv";
}  // namespace AppConfig

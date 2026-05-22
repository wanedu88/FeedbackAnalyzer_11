#pragma once
#include <string>
#include <vector>

class SentimentClassifier {
public:
    static bool containsAny(const std::string& text, const std::vector<std::string>& keywords);
    static std::string classifySentiment(const std::string& text);
};

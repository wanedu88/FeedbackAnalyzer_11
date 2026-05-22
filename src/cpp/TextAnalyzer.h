#pragma once
#include <string>
#include <vector>
#include <map>
#include "Feedback.h"
#include "Constants.h"
#include "SentimentClassifier.h"

class TextAnalyzer {
private:
    static std::map<std::string, int> globalSent;
    static std::map<std::string, int> globalKw;

public:
    std::map<std::string, int> sent(const std::vector<Feedback>& feedbacks);
    std::map<std::string, int> kw(const std::vector<Feedback>& feedbacks);
};

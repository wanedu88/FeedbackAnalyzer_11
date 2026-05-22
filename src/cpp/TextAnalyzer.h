#pragma once
#include <string>
#include <vector>
#include <map>
#include "Feedback.h"

class TextAnalyzer {
public:
    std::map<std::string, int> countSentiments(const std::vector<Feedback>& feedbacks);
    std::map<std::string, int> countKeywords(const std::vector<Feedback>& feedbacks);
};

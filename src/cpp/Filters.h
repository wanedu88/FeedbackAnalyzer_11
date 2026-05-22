#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Feedback.h"
#include "Constants.h"
#include "SentimentClassifier.h"

class Filters {
private:
    static std::map<std::string, std::vector<std::string>> S_KEYWORDS;

public:
    static void initFilterKeywords();

    std::vector<Feedback> fil(const std::vector<Feedback>& dataList,
                              const std::string& sFilter,
                              const std::string& kFilter);
};

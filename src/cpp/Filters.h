#pragma once
#include <string>
#include <vector>
#include "Feedback.h"

class Filters {
public:
    std::vector<Feedback> filterFeedbacks(const std::vector<Feedback>& dataList,
                                          const std::string& sentimentFilter,
                                          const std::string& keywordFilter);
};

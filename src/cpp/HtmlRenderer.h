#pragma once

#include "Feedback.h"

#include <map>
#include <string>
#include <vector>

class HtmlRenderer {
public:
    static std::string renderPage(const std::string& success,
                                  const std::string& warning,
                                  const std::string& error,
                                  const std::map<std::string, int>& sentimentResults,
                                  const std::map<std::string, int>& keywordResults,
                                  const std::vector<Feedback>& feedbacks);
};

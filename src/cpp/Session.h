#pragma once
#include <vector>
#include <map>
#include <string>
#include "Feedback.h"

class Session {
private:
    static std::vector<Feedback> currentFeedbacks;
    static std::vector<Feedback> filteredFeedbacks;
    static std::map<std::string, std::string> internalData;
    static std::map<std::string, std::string> filterOptions;

public:
    static void initSessionState() {
        // already initialized as static
    }

    static std::vector<Feedback>& getCurrentFeedbacks() {
        return currentFeedbacks;
    }

    static void updateCurrentFeedbacks(const std::vector<Feedback>& feedbacks) {
        currentFeedbacks = feedbacks;
    }

    static void setFilteredFeedbacks(const std::vector<Feedback>& feedbacks) {
        filteredFeedbacks = feedbacks;
    }

    static const std::vector<Feedback>& getFilteredFeedbacks() {
        return filteredFeedbacks;
    }
};

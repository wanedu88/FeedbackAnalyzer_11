#include "Filters.h"
#include "Constants.h"
#include "SentimentClassifier.h"
#include <iostream>

std::vector<Feedback> Filters::filterFeedbacks(const std::vector<Feedback>& dataList,
                                               const std::string& sentimentFilter,
                                               const std::string& keywordFilter) {
    std::vector<Feedback> tmpFiltered;

    if (sentimentFilter != u8"전체") {
        for (const auto& item : dataList) {
            const std::string currentSentiment =
                SentimentClassifier::classifySentiment(item.getText());
            if (currentSentiment == sentimentFilter) {
                tmpFiltered.push_back(item);
            }
        }
    } else {
        tmpFiltered = dataList;
    }

    std::vector<Feedback> finalFiltered;
    if (keywordFilter != u8"전체") {
        for (const auto& item : tmpFiltered) {
            std::string txt = item.getText();
            if (Constants::CATEGORY_KEYWORDS.count(keywordFilter)) {
                const auto& catMap = Constants::CATEGORY_KEYWORDS[keywordFilter];
                for (const auto& subEntry : catMap) {
                    if (SentimentClassifier::containsAny(txt, subEntry.second)) {
                        finalFiltered.push_back(item);
                        break;
                    }
                }
            }
        }
    } else {
        finalFiltered = tmpFiltered;
    }

    for (const auto& i : finalFiltered) {
        std::cout << i.getText() << std::endl;
    }

    return finalFiltered;
}

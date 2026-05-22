#include "TextAnalyzer.h"
#include "Constants.h"
#include "SentimentClassifier.h"

std::map<std::string, int> TextAnalyzer::countSentiments(
    const std::vector<Feedback>& feedbacks) {
    std::map<std::string, int> res;
    res[u8"긍정"] = 0;
    res[u8"중립"] = 0;
    res[u8"부정"] = 0;

    for (const auto& f : feedbacks) {
        const std::string s = SentimentClassifier::classifySentiment(f.getText());
        res[s]++;
    }

    return res;
}

std::map<std::string, int> TextAnalyzer::countKeywords(
    const std::vector<Feedback>& feedbacks) {
    std::map<std::string, int> res2;
    for (const auto& entry : Constants::CATEGORY_KEYWORDS) {
        res2[entry.first] = 0;
    }

    for (const auto& f : feedbacks) {
        std::string txt = f.getText();
        for (const auto& entry : Constants::CATEGORY_KEYWORDS) {
            const std::string& cat = entry.first;
            if (entry.second.count("main")) {
                const auto& kws = entry.second.at("main");
                if (SentimentClassifier::containsAny(txt, kws)) {
                    res2[cat]++;
                }
            }
        }
    }

    return res2;
}

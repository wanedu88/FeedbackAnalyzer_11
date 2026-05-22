#include "SentimentClassifier.h"
#include "Constants.h"

bool SentimentClassifier::containsAny(const std::string& text,
                                      const std::vector<std::string>& keywords) {
    for (const auto& kw : keywords) {
        if (text.find(kw) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string SentimentClassifier::classifySentiment(const std::string& text) {
    if (containsAny(text, Constants::SENTIMENT_KEYWORDS[u8"긍정"])) {
        return u8"긍정";
    }
    if (containsAny(text, Constants::SENTIMENT_KEYWORDS[u8"부정"])) {
        return u8"부정";
    }
    return u8"중립";
}

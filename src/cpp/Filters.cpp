#include "Filters.h"

std::map<std::string, std::vector<std::string>> Filters::S_KEYWORDS;

void Filters::initFilterKeywords() {
    S_KEYWORDS[u8"긍정"] = {
        u8"좋아요", u8"만족", u8"감사", u8"친절", u8"좋다", u8"좋았", u8"좋은", u8"우수",
        u8"빠르", u8"정확", u8"신속", u8"안전", u8"괜찮", u8"인상적", u8"추천", u8"기대 이상",
        u8"합리", u8"꼼꼼", u8"뛰어납니다", u8"만족스럽", u8"좋았습니다", u8"좋습니다",
        u8"만족합니다", u8"굿", u8"최고", u8"최고입니다", u8"감사합니다"
    };
    S_KEYWORDS[u8"부정"] = {
        u8"나쁘", u8"불만", u8"실망", u8"최악", u8"별로", u8"불편", u8"불만족", u8"문제",
        u8"불량", u8"불량품", u8"환불", u8"교환", u8"불만족스럽", u8"실망스럽",
        u8"비싸", u8"불친절", u8"늦다"
    };
    S_KEYWORDS[u8"중립"] = {
        u8"괜찮", u8"보통", u8"평범", u8"무난", u8"그냥", u8"전반적", u8"완료",
        u8"적당", u8"나쁘지 않", u8"특별", u8"없"
    };
}

std::vector<Feedback> Filters::fil(const std::vector<Feedback>& dataList,
                                     const std::string& sFilter,
                                     const std::string& kFilter) {
    std::vector<Feedback> tmpFiltered;

    if (sFilter != u8"전체") {
        for (const auto& item : dataList) {
            const std::string currentSentiment =
                SentimentClassifier::classifySentiment(item.getText());
            if (currentSentiment == sFilter) {
                tmpFiltered.push_back(item);
            }
        }
    } else {
        tmpFiltered = dataList;
    }

    std::vector<Feedback> finalFiltered;
    if (kFilter != u8"전체") {
        for (const auto& item : tmpFiltered) {
            std::string txt = item.getText();
            if (Constants::CATEGORY_KEYWORDS.count(kFilter)) {
                const auto& catMap = Constants::CATEGORY_KEYWORDS[kFilter];
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

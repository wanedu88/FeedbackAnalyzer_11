#include "httplib.h"
#include "Feedback.h"
#include "Constants.h"
#include "Session.h"
#include "TextAnalyzer.h"
#include "Filters.h"
#include "FileHandler.h"
#include "Logger.h"
#include "ParseUtils.h"
#include "HtmlRenderer.h"
#include "AppConfig.h"
#include <sstream>
#include <fstream>

static TextAnalyzer textAnalyzer;
static Filters filters;
static FileHandler fileHandler;

static void respondHtml(httplib::Response& res, const std::string& html) {
    res.set_content(html, "text/html; charset=UTF-8");
}

static void handleGetRoot(const httplib::Request&, httplib::Response& res) {
    Session::initSessionState();
    auto& feedbacks = Session::getCurrentFeedbacks();
    respondHtml(res, HtmlRenderer::renderPage(u8"피드백 분석기 시작", "", "", {}, {}, feedbacks));
}

static void handlePostAnalyze(const httplib::Request& req, httplib::Response& res) {
    Logger::clearPageMessages();
    try {
        auto& feedbacks = Session::getCurrentFeedbacks();
        auto params = ParseUtils::parseForm(req.body);
        std::string text = params["text"];

        if (!text.empty()) {
            auto start = text.find_first_not_of(" \t\r\n");
            auto end = text.find_last_not_of(" \t\r\n");
            if (start != std::string::npos) {
                text = text.substr(start, end - start + 1);
                feedbacks.push_back(Feedback(text));
            }
        }

        for (const auto& fb : feedbacks) {
            Logger::logInfo(fb.getText());
        }

        Logger::logInfo(u8"현재 " + std::to_string(feedbacks.size()) + u8"개의 피드백이 입력되었습니다.");

        std::string success = std::to_string(feedbacks.size()) + u8"개의 피드백이 입력되었습니다.";
        std::map<std::string, int> sentimentResults;
        std::map<std::string, int> keywordResults;

        if (!feedbacks.empty()) {
            sentimentResults = textAnalyzer.countSentiments(feedbacks);
            keywordResults = textAnalyzer.countKeywords(feedbacks);
            Logger::logInfo(u8"감성 분석 완료");
            Logger::logInfo(u8"키워드 분석 완료");
        }

        respondHtml(res, HtmlRenderer::renderPage(success, "", "", sentimentResults, keywordResults, feedbacks));
    } catch (const std::exception& e) {
        Logger::logError(std::string(u8"오류 발생: ") + e.what());
        respondHtml(res, HtmlRenderer::renderPage("", "", Logger::getPageError(), {}, {}, {}));
    }
}

static void handlePostUpload(const httplib::Request& req, httplib::Response& res) {
    Logger::clearPageMessages();
    try {
        auto& feedbacks = Session::getCurrentFeedbacks();
        if (req.form.has_file("file")) {
            const auto file = req.form.get_file("file");
            if (!file.content.empty()) {
                std::istringstream stream(file.content);
                std::string line;
                bool firstLine = true;
                while (std::getline(stream, line)) {
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    if (firstLine) {
                        firstLine = false;
                        continue;
                    }
                    if (line.empty()) {
                        continue;
                    }
                    auto fields = ParseUtils::parseCsvLine(line);
                    if (!fields.empty() && !fields[0].empty()) {
                        feedbacks.push_back(Feedback(fields[0]));
                    }
                }
                Logger::logInfo(u8"파일이 성공적으로 업로드되었습니다.");
            }
        }
        std::string success = std::to_string(feedbacks.size()) + u8"개의 피드백이 입력되었습니다.";
        respondHtml(res, HtmlRenderer::renderPage(success, "", "", {}, {}, feedbacks));
    } catch (const std::exception& e) {
        Logger::logError(std::string(u8"파일 업로드 오류: ") + e.what());
        respondHtml(res, HtmlRenderer::renderPage("", "", Logger::getPageError(), {}, {}, {}));
    }
}

static void handlePostFilter(const httplib::Request& req, httplib::Response& res) {
    Logger::clearPageMessages();
    try {
        auto& feedbacks = Session::getCurrentFeedbacks();
        auto params = ParseUtils::parseForm(req.body);
        std::string sentiment = params["sentiment"];
        std::string keyword = params["keyword"];

        if (feedbacks.empty()) {
            Logger::logWarning(u8"분석할 피드백이 없습니다.");
            respondHtml(res, HtmlRenderer::renderPage("", Logger::getPageWarning(), "", {}, {}, {}));
            return;
        }

        auto filtered = filters.filterFeedbacks(feedbacks, sentiment, keyword);
        if (filtered.empty()) {
            Logger::logWarning(u8"필터링 결과가 없습니다.");
            respondHtml(res, HtmlRenderer::renderPage("", Logger::getPageWarning(), "", {}, {}, {}));
            return;
        }

        Session::setFilteredFeedbacks(filtered);
        auto sentimentResults = textAnalyzer.countSentiments(filtered);
        auto keywordResults = textAnalyzer.countKeywords(filtered);
        Logger::logInfo(u8"필터링 결과: " + std::to_string(filtered.size()) + u8"개의 피드백");
        respondHtml(res, HtmlRenderer::renderPage("", "", "", sentimentResults, keywordResults, filtered));
    } catch (const std::exception& e) {
        Logger::logError(std::string(u8"오류 발생: ") + e.what());
        respondHtml(res, HtmlRenderer::renderPage("", "", Logger::getPageError(), {}, {}, {}));
    }
}

static void handleGetDownload(const httplib::Request&, httplib::Response& res) {
    std::ostringstream csv;
    csv << "\xEF\xBB\xBF";
    csv << "text\n";
    for (const auto& iter : Session::getFilteredFeedbacks()) {
        csv << ParseUtils::escapeCsvField(iter.getText()) << "\n";
    }
    res.set_header("Content-Disposition", "attachment; filename=\"filtered_feedback.csv\"");
    res.set_content(csv.str(), "text/csv; charset=UTF-8");
}

int main() {
    Constants::init();

    httplib::Server svr;

    svr.Get("/", handleGetRoot);
    svr.Post("/analyze", handlePostAnalyze);
    svr.Post("/upload", handlePostUpload);
    svr.Post("/filter", handlePostFilter);
    svr.Get("/download", handleGetDownload);

    Logger::logInfo(u8"서버가 http://localhost:" + std::to_string(AppConfig::kServerPort) +
                    u8" 에서 시작됩니다.");
    svr.listen(AppConfig::kServerHost, AppConfig::kServerPort);

    return 0;
}

#include "HtmlRenderer.h"

#include "AppConfig.h"
#include "ParseUtils.h"
#include "UIComponents.h"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace {

std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void appendPageHead(std::ostringstream& html) {
    html << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Feedback Analyzer</title>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }
        .container { max-width: 1200px; margin: 0 auto; background-color: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; text-align: center; margin-bottom: 30px; }
        .section { margin-bottom: 30px; padding: 20px; border: 1px solid #ddd; border-radius: 5px; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; font-weight: bold; color: #555; }
        input[type="text"], textarea, select { width: 100%; padding: 10px; border: 1px solid #ddd; border-radius: 4px; font-size: 14px; box-sizing: border-box; }
        textarea { height: )" << AppConfig::kTextareaHeightPx << R"(px; resize: vertical; }
        button { background-color: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 14px; margin-right: 10px; }
        button:hover { background-color: #0056b3; }
        .btn-success { background-color: #28a745; }
        .btn-success:hover { background-color: #1e7e34; }
        .alert-success { background-color: #d4edda; border-color: #c3e6cb; color: #155724; padding: 10px; border-radius: 4px; }
        .alert-warning { background-color: #fff3cd; border-color: #ffeaa7; color: #856404; padding: 10px; border-radius: 4px; }
        .alert-danger { background-color: #f8d7da; border-color: #f5c6cb; color: #721c24; padding: 10px; border-radius: 4px; }
        .stats { display: flex; justify-content: space-around; margin: 20px 0; }
        .stat-item { text-align: center; padding: 15px; background-color: #f8f9fa; border-radius: 5px; flex: 1; margin: 0 10px; }
        .stat-number { font-size: 24px; font-weight: bold; color: #007bff; }
        .stat-label { color: #666; margin-top: 5px; }
        .feedback-list { list-style: none; padding: 0; margin: 0; }
        .feedback-item { padding: 10px; margin-bottom: 8px; background-color: #f8f9fa; border-radius: 4px; white-space: pre-wrap; word-break: break-word; }
    </style>
</head>
<body>
<div class="container">
    <h1>Feedback Analyzer</h1>
    <p style="text-align: center; color: #666;">)" << u8"고객 피드백 분석 시스템" << R"(</p>)";
}

void appendSuccessAlert(std::ostringstream& html, const std::string& success) {
    if (success.empty()) {
        return;
    }
    html << R"(<p class="alert alert-success">)" << getCurrentTimestamp() << " : "
         << ParseUtils::escapeHtml(success) << "</p>";
}

void appendWarningAlert(std::ostringstream& html, const std::string& warning) {
    if (warning.empty()) {
        return;
    }
    html << R"(<p class="alert alert-warning">)" << ParseUtils::escapeHtml(warning) << "</p>";
}

void appendErrorAlert(std::ostringstream& html, const std::string& error) {
    if (error.empty()) {
        return;
    }
    html << R"(<p class="alert alert-danger">)" << ParseUtils::escapeHtml(error) << "</p>";
}

void appendInputSection(std::ostringstream& html) {
    html << R"(
    <div class="section">
        <h3>)" << u8"피드백 입력" << R"(</h3>
        <form action="/analyze" method="post">
            <div class="form-group">
                <label for="text">)" << u8"피드백 텍스트:" << R"(</label>
                <textarea id="text" name="text" placeholder=")" << u8"피드백을 입력하세요..." << R"("></textarea>
            </div>
            <button type="submit">)" << u8"입력하기" << R"(</button>
        </form>
    </div>)";
}

void appendUploadSection(std::ostringstream& html) {
    html << R"(
    <div class="section">
        <h3>)" << u8"CSV 파일 업로드 (선택사항)" << R"(</h3>
        <form action="/upload" method="post" enctype="multipart/form-data">
            <div class="form-group">
                <label for="file">)" << u8"CSV 파일 선택:" << R"(</label>
                <input type="file" id="file" name="file" accept=".csv">
            </div>
            <button type="submit">)" << u8"업로드" << R"(</button>
        </form>
    </div>)";
}

void appendFilterSection(std::ostringstream& html) {
    const auto& cats = UIComponents::getCategories();
    html << R"(
    <div class="section">
        <h3>)" << u8"피드백 분석" << R"(</h3>
        <form action="/filter" method="post">
            <div class="form-group">
                <label for="sentiment">)" << u8"감정 필터:" << R"(</label>
                <select id="sentiment" name="sentiment">
                    <option value=")" << u8"전체" << R"(">)" << u8"전체" << R"(</option>
                    <option value=")" << u8"긍정" << R"(">)" << u8"긍정" << R"(</option>
                    <option value=")" << u8"중립" << R"(">)" << u8"중립" << R"(</option>
                    <option value=")" << u8"부정" << R"(">)" << u8"부정" << R"(</option>
                </select>
            </div>
            <div class="form-group">
                <label for="keyword">)" << u8"키워드 필터:" << R"(</label>
                <select id="keyword" name="keyword">
                    <option value=")" << u8"전체" << R"(">)" << u8"전체" << R"(</option>)";
    for (const auto& cat : cats) {
        html << R"(<option value=")" << cat << R"(">)" << cat << "</option>";
    }
    html << R"(
                </select>
            </div>
            <button type="submit">)" << u8"분  석" << R"(</button>
        </form>
    </div>)";
}

void appendStatsBlock(std::ostringstream& html,
                      const char* title,
                      const std::map<std::string, int>& results) {
    if (results.empty()) {
        return;
    }
    html << "<h4>" << title << R"(</h4><div class="stats">)";
    for (const auto& entry : results) {
        html << R"(<div class="stat-item"><div class="stat-number">)" << entry.second
             << R"(</div><div class="stat-label">)" << entry.first << "</div></div>";
    }
    html << "</div>";
}

void appendStatsSection(std::ostringstream& html,
                        const std::map<std::string, int>& sentimentResults,
                        const std::map<std::string, int>& keywordResults) {
    if (sentimentResults.empty() && keywordResults.empty()) {
        return;
    }
    html << R"(<div class="section"><h3>)" << u8"분석 결과" << "</h3>";
    appendStatsBlock(html, u8"감정 분포", sentimentResults);
    appendStatsBlock(html, u8"키워드 분포", keywordResults);
    html << R"(<a href="/download"><button class="btn-success">)" << u8"결과 다운로드"
         << "</button></a></div>";
}

void appendFeedbackList(std::ostringstream& html, const std::vector<Feedback>& feedbacks) {
    if (feedbacks.empty()) {
        return;
    }
    html << R"(<div class="section"><h3>)" << u8"피드백 목록" << R"(</h3><ul class="feedback-list">)";
    for (const auto& fb : feedbacks) {
        html << R"(<li class="feedback-item">)" << ParseUtils::escapeHtml(fb.getText()) << "</li>";
    }
    html << "</ul></div>";
}

}  // namespace

std::string HtmlRenderer::renderPage(const std::string& success,
                                     const std::string& warning,
                                     const std::string& error,
                                     const std::map<std::string, int>& sentimentResults,
                                     const std::map<std::string, int>& keywordResults,
                                     const std::vector<Feedback>& feedbacks) {
    std::ostringstream html;
    appendPageHead(html);
    appendSuccessAlert(html, success);
    appendInputSection(html);
    appendUploadSection(html);
    appendFilterSection(html);
    appendWarningAlert(html, warning);
    appendStatsSection(html, sentimentResults, keywordResults);
    appendFeedbackList(html, feedbacks);
    appendErrorAlert(html, error);
    html << "</div></body></html>";
    return html.str();
}

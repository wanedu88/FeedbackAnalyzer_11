#include "services/TrendLoader.h"

#include "ParseUtils.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace {

std::string trimAscii(const std::string& value) {
    size_t start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }
    return value.substr(start, end - start);
}

std::string toLowerAscii(const std::string& value) {
    std::string out = value;
    for (char& ch : out) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return out;
}

bool isValidDateKey(const std::string& date) {
    if (date.size() != 10) {
        return false;
    }
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') {
                return false;
            }
        } else if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }
    return true;
}

std::string normalizeDate(const std::string& raw) {
    std::string value = trimAscii(raw);
    const size_t spacePos = value.find(' ');
    if (spacePos != std::string::npos) {
        value = value.substr(0, spacePos);
    }
    if (value.size() >= 10) {
        value = value.substr(0, 10);
    }
    return isValidDateKey(value) ? value : std::string{};
}

bool isKnownSentiment(const std::string& sentiment) {
    return sentiment == u8"긍정" || sentiment == u8"부정" || sentiment == u8"중립";
}

struct ColumnMap {
    int date{-1};
    int text{-1};
    int sentiment{-1};
};

ColumnMap mapHeader(const std::vector<std::string>& fields) {
    ColumnMap map;
    for (size_t i = 0; i < fields.size(); ++i) {
        const std::string key = toLowerAscii(trimAscii(fields[i]));
        if (key == "date" || key == "timestamp") {
            map.date = static_cast<int>(i);
        } else if (key == "text") {
            map.text = static_cast<int>(i);
        } else if (key == "sentiment") {
            map.sentiment = static_cast<int>(i);
        }
    }
    return map;
}

bool parseTrendRows(const std::vector<std::string>& lines, std::vector<TrendRecord>& out) {
    out.clear();
    if (lines.empty()) {
        return false;
    }

    const auto headerFields = ParseUtils::parseCsvLine(lines[0]);
    const ColumnMap columns = mapHeader(headerFields);
    if (columns.date < 0 || columns.text < 0) {
        return false;
    }

    for (size_t i = 1; i < lines.size(); ++i) {
        if (lines[i].empty()) {
            continue;
        }
        const auto fields = ParseUtils::parseCsvLine(lines[i]);
        if (static_cast<int>(fields.size()) <= std::max(columns.date, columns.text)) {
            continue;
        }

        TrendRecord record;
        record.date = normalizeDate(fields[static_cast<size_t>(columns.date)]);
        record.text = trimAscii(fields[static_cast<size_t>(columns.text)]);
        if (record.date.empty() || record.text.empty()) {
            continue;
        }
        if (columns.sentiment >= 0 &&
            static_cast<int>(fields.size()) > columns.sentiment) {
            record.sentimentOverride = trimAscii(fields[static_cast<size_t>(columns.sentiment)]);
            if (!record.sentimentOverride.empty() && !isKnownSentiment(record.sentimentOverride)) {
                record.sentimentOverride.clear();
            }
        }
        out.push_back(std::move(record));
    }

    return !out.empty();
}

std::string readFileUtf8(const std::string& path) {
    std::ifstream file(std::filesystem::path(path), std::ios::binary);
    if (!file) {
        return {};
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    if (content.size() >= 3 && static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF) {
        content.erase(0, 3);
    }
    return content;
}

std::vector<std::string> splitLines(const std::string& content) {
    std::vector<std::string> lines;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

}  // namespace

bool TrendLoader::loadFromContent(const std::string& csvContent, std::vector<TrendRecord>& out) {
    if (csvContent.empty()) {
        return false;
    }
    return parseTrendRows(splitLines(csvContent), out);
}

bool TrendLoader::load(const std::string& path, std::vector<TrendRecord>& out) {
    const std::string content = readFileUtf8(path);
    if (content.empty()) {
        return false;
    }
    return loadFromContent(content, out);
}

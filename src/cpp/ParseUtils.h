#pragma once

#include <map>
#include <string>
#include <vector>

namespace ParseUtils {

std::string urlDecode(const std::string& str);
std::vector<std::string> parseCsvLine(const std::string& line);
std::map<std::string, std::string> parseForm(const std::string& body);
std::string escapeHtml(const std::string& s);
std::string escapeCsvField(const std::string& s);

}  // namespace ParseUtils

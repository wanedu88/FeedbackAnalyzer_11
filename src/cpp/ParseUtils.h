#pragma once

#include <string>
#include <vector>

namespace ParseUtils {

std::string urlDecode(const std::string& str);
std::vector<std::string> parseCsvLine(const std::string& line);

}  // namespace ParseUtils

#include "Session.h"

std::vector<Feedback> Session::currentFeedbacks;
std::vector<Feedback> Session::filteredFeedbacks;
std::map<std::string, std::string> Session::internalData;
std::map<std::string, std::string> Session::filterOptions;

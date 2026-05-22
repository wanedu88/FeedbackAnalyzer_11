#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

class Logger {
private:
    static bool debugMode;
    static std::string pageWarning;
    static std::string pageError;

    static std::string getTimestamp() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

public:
    static void clearPageMessages() {
        pageWarning.clear();
        pageError.clear();
    }

    static const std::string& getPageWarning() { return pageWarning; }
    static const std::string& getPageError() { return pageError; }

    static void logInfo(const std::string& message) {
        std::cout << "[" << getTimestamp() << "] INFO: " << message << std::endl;
    }

    static void logWarning(const std::string& message) {
        pageWarning = message;
        std::cout << "[" << getTimestamp() << "] WARNING: " << message << std::endl;
    }

    static void logError(const std::string& message) {
        pageError = message;
        std::cerr << "[" << getTimestamp() << "] ERROR: " << message << std::endl;
    }

    static void logDebug(const std::string& message) {
        if (debugMode) {
            std::cout << "[" << getTimestamp() << "] DEBUG: " << message << std::endl;
        }
    }

    static void setDebugMode(bool mode) { debugMode = mode; }
    static bool isDebugMode() { return debugMode; }
};

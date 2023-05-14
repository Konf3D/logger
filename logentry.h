#include <iostream>
#include <string>
#include <chrono>
#include <unordered_map>
#include "timestamp.h"
enum class SeverityLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class LogEntry {
public:
    LogEntry(const std::string& message, SeverityLevel severityLevel)
        : message(message), severityLevel(severityLevel) {
        timestamp = time_stamp();
    }

    void setLogger(const std::string& logger) {
        this->logger = logger;
    }

    void setThreadID(int threadID) {
        this->threadID = threadID;
    }

    void setAdditionalMetadata(const std::string& key, const std::string& value) {
        additionalMetadata[key] = value;
    }

    operator std::string() const {
        std::string logString;
        logString += "[" + timestamp + "]";
        logString += " [" + severityLevelToString(severityLevel) + "]";
        if (!logger.empty()) {
            logString += " [" + logger + "]";
        }
        if (threadID >= 0) {
            logString += " [Thread ID: " + std::to_string(threadID) + "]";
        }
        for (const auto& data : additionalMetadata) {
            logString += " [" + data.first + ": " + data.second + "]";
        }
        logString += " " + message;
        return logString;
    }

private:

    std::string severityLevelToString(SeverityLevel level) const {
        switch (level) {
        case SeverityLevel::DEBUG:
            return "DEBUG";
        case SeverityLevel::INFO:
            return "INFO";
        case SeverityLevel::WARNING:
            return "WARNING";
        case SeverityLevel::ERROR:
            return "ERROR";
        case SeverityLevel::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string timestamp;
    std::string message;
    SeverityLevel severityLevel;
    std::string logger;
    int threadID = -1;
    std::unordered_map<std::string, std::string> additionalMetadata;
};
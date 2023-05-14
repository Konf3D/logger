#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <exception>
#include <queue>

class Logger {
public:
    Logger(const std::string& file_path) : file_path(file_path) {
        if (!openFile())
            openFail();
    }

    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }

    void writeLog(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        if (file.is_open() || openFile()) {
            file << message << '\n';
            logs.push(message);  // Store the log entry in memory
            return;
        }
        return openFail();
    }

    std::string readLog() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!logs.empty()) {
            std::string logEntry = logs.front();  // Retrieve the next log entry from memory
            logs.pop();  // Remove the retrieved log entry from memory
            return logEntry;
        }
        return "";
    }

    std::string readSpecificLine(unsigned int line_number) {
        if (!line_number)
            return std::string();
        file.seekg(0);
        std::lock_guard<std::mutex> lock(mutex);
        std::ifstream inputFile(file_path);
        std::string line;
        unsigned int current_line = 0;

        if (!inputFile.is_open()) {
            std::cout << "Error: Log file could not be opened." << std::endl;
            return line;
        }

        while (current_line < line_number && std::getline(inputFile, line)) {
            current_line++;
        }

        inputFile.close();
        return line;
    }

private:
    //try to open file in case of failure
    bool openFile()
    {
        if(!file.is_open())
            file.open(file_path, std::ios::app);
        return file.is_open();
    }
    //throw exception if file cannot be opened or reopened
    void openFail()
    {
        throw std::ios_base::failure("Failed to open file: " + file_path);
    }
    std::string file_path;
    std::fstream file;
    std::mutex mutex;
    std::queue<std::string> logs;
};
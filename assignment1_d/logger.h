#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <memory>

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();
    void log(const std::string& message);
    void logError(const std::string& message);
private:
    Logger() = default;

    std::mutex log_mutex;
    static std::unique_ptr<Logger> instance;
    static std::once_flag flag;
};
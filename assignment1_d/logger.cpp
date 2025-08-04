#include "logger.h"

std::unique_ptr<Logger> Logger::instance;
std::once_flag Logger::flag;

Logger& Logger::getInstance() {
    std::call_once(flag, []() {
        instance.reset(new Logger());
    });
    return *instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "INFO: " << message << std::endl;
}

void Logger::logError(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cerr << "ERROR: " << message << std::endl;
}
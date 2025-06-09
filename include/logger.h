#ifndef QPP_LOGGER_H
#define QPP_LOGGER_H

#include <iostream>
#include <mutex>
#include <string>

namespace qpp {

enum class LogLevel { Error = 0, Warning, Info, Debug };

inline LogLevel current_log_level = LogLevel::Info;
inline std::mutex log_mtx;

inline void set_log_level(LogLevel level) { current_log_level = level; }

inline const char* level_to_string(LogLevel level) {
    switch (level) {
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Warning:
        return "WARN";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Debug:
        return "DEBUG";
    }
    return "";
}

template <typename... Args>
void log(LogLevel level, Args&&... args) {
    if (level > current_log_level) return;
    std::lock_guard<std::mutex> lock(log_mtx);
    std::cerr << "[" << level_to_string(level) << "] ";
    (std::cerr << ... << args) << std::endl;
}

#define LOG_ERROR(...) ::qpp::log(::qpp::LogLevel::Error, __VA_ARGS__)
#define LOG_WARN(...)  ::qpp::log(::qpp::LogLevel::Warning, __VA_ARGS__)
#define LOG_INFO(...)  ::qpp::log(::qpp::LogLevel::Info, __VA_ARGS__)
#define LOG_DEBUG(...) ::qpp::log(::qpp::LogLevel::Debug, __VA_ARGS__)

} // namespace qpp

#endif // QPP_LOGGER_H

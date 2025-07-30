#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// Log levels
enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

enum class Color {Red, Green, Blue, Yellow, Magenta ,White};

inline const char* LogLevelToString(LogLevel level) {
    switch(level) {
    case LogLevel::TRACE: return "TRACE";
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO:  return "INFO";
    case LogLevel::WARN:  return "WARN";
    case LogLevel::ERROR: return "ERROR";
    case LogLevel::FATAL: return "FATAL";
    default: return "UNKNOWN";
    }
}

// Log message sink interface
class LogSink {
  public:
    virtual ~LogSink() = default;
    virtual void log(const std::string& message) = 0;
};

// Console output sink
class ConsoleSink : public LogSink {
  public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

struct LogColors{
    Color traceLogColor{Color::White};
    Color debugLogColor{Color::Green};
    Color infoLogColor{Color::Blue};
    Color warningLogColor{Color::Yellow};
    Color errorLogColor{Color::Magenta};
    Color fatalLogColor{Color::Red};
};

struct ShouldAdd{
    bool timeStamp{false};
    bool LogLevel{false};
    bool FileInfo{false};
};

struct Config{
    LogColors logColors;
    ShouldAdd shouldAdd;
    bool useBoldText{false};
};

// Logger class
class Logger {
  public:
    Logger(const Logger&);

    Logger& operator=(const Logger&);

    void log(LogLevel level, const std::string& msg,
             Config config, const char* file, int line, const char* func)
    {

        std::string formatted = formatMessage(level, msg,config, file, line, func);

        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << formatted << std::endl;
    }
    Logger() : level_(LogLevel::TRACE) {}

  private:
    std::string formatMessage(LogLevel level, const std::string& msg,
                              Config config, const char* file, int line, const char* func)
    {
        // Timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string boldFormatter = config.useBoldText? "[1;" : "[0;";
        std::stringstream ss;
        Color logColor = getLogColor(level, config);
        std::string logColorAnsi = getLogColorAnsi(logColor);
        ss << "\033"<<boldFormatter << logColorAnsi <<"[" << std::put_time(std::localtime(&t), "%F %T") << "] ";
        ss << "[" << LogLevelToString(level) << "] ";
        ss << "[" << file << ":" << line << " (" << func << ")]";
        ss << msg<< "\033[0m\n";
        return ss.str();
    }
    Color getLogColor(LogLevel level, Config config){
        switch (level) {
        case LogLevel::DEBUG:
            return config.logColors.debugLogColor;
        case LogLevel::ERROR:
            return config.logColors.errorLogColor;
        case LogLevel::FATAL:
            return config.logColors.fatalLogColor;
        case LogLevel::INFO:
            return config.logColors.infoLogColor;
        case LogLevel::TRACE:
            return config.logColors.traceLogColor;
        case LogLevel::WARN:
            return config.logColors.warningLogColor;
        default:
            return Color::White;
        }
    }
    std::string getLogColorAnsi(Color color){
        switch (color) {
        case Color::Blue:
            return "34m";
        case Color::Green:
            return "32m";
        case Color::Magenta:
            return "35m";
        case Color::Red:
            return "31m";
        case Color::White:
            return "37m";
        case Color::Yellow:
            return "33m";
        default:
            return "";
        }
    }


    LogLevel level_;
    std::mutex mutex_;
};

Logger logger;
// Convenience macros
#define LOG_TRACE(msg,config) logger.log(LogLevel::TRACE, msg, config,__FILE__, __LINE__, __func__)
#define LOG_DEBUG(msg,config) logger.log(LogLevel::DEBUG, msg, config,__FILE__, __LINE__, __func__)
#define LOG_INFO(msg,config)  logger.log(LogLevel::INFO,  msg, config,__FILE__, __LINE__, __func__)
#define LOG_WARN(msg,config)  logger.log(LogLevel::WARN,  msg, config,__FILE__, __LINE__, __func__)
#define LOG_ERROR(msg,config) logger.log(LogLevel::ERROR, msg, config,__FILE__, __LINE__, __func__)
#define LOG_FATAL(msg,config) logger.log(LogLevel::FATAL, msg, config,__FILE__, __LINE__, __func__)

#endif // LOGGER_H

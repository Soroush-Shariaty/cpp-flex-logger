#ifndef CPP_FLEX_LOGGER_H
#define CPP_FLEX_LOGGER_H

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <vector>

namespace cpp_flex_logger
{
enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

enum class Color
{
    Red,
    Green,
    Blue,
    Yellow,
    Magenta,
    White
};

enum class Output
{
    Console,
    File
};

enum class LogContent
{
    TimeStamp,
    LogLevel,
    LogLocation,
    Message,
};

inline const char *LogLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warn:
        return "WARN";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

struct LogColors
{
    Color traceLogColor{Color::White};
    Color debugLogColor{Color::Green};
    Color infoLogColor{Color::Blue};
    Color warningLogColor{Color::Yellow};
    Color errorLogColor{Color::Magenta};
    Color fatalLogColor{Color::Red};
};

struct ConsoleLog
{
    bool enable{true};
    LogColors logColors;
    bool useBoldText{false};
};

struct FileLog
{
    bool enable{false};
    std::string absoluteFileLocation{"log.txt"};
};

struct Config
{
    std::vector<LogContent> logContentList{LogContent::LogLevel, LogContent::Message};
    ConsoleLog consoleLog;
    FileLog fileLog;
};

// Logger class
class Logger
{
  public:
    Logger() {}

    static void Log(LogLevel level, const std::string &msg, Config config, const char *file, int line, const char *func)
    {
        std::once_flag invalidFileFlag;
        Logger logger;
        if (config.consoleLog.enable)
        {
            std::string formatted = logger.FormatMessage(level, msg, config, file, line, func, Output::Console);
            std::cout << formatted << std::endl;
        }
        if (config.fileLog.enable)
        {
            std::ofstream logFile(config.fileLog.absoluteFileLocation, std::ios::app);
            if (!logFile)
            { // Check if file opened successfully
                std::call_once(invalidFileFlag,
                               [config]()
                               {
                                   std::cerr << "Error opening file \"" << config.fileLog.absoluteFileLocation
                                             << "\" for writing. Falling back to "
                                                "default \"log.txt\" in "
                                                "the build directory.\n";
                               });
                logFile.close();
                logFile.open("log.txt", std::ios::app);
            }
            std::string formatted = logger.FormatMessage(level, msg, config, file, line, func, Output::File);
            logFile << formatted << std::endl;

            logFile.close();
        }
    }

  private:
    std::string FormatMessage(LogLevel level, const std::string &msg, Config config, const char *file, int line, const char *func, Output output)
    {
        // Timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        for (auto item : config.logContentList)
        {
            switch (item)
            {
            case LogContent::LogLevel:
                ss << "[" << LogLevelToString(level) << "] ";
                break;
            case LogContent::TimeStamp:
                ss << "[" << std::put_time(std::localtime(&t), "%F %T") << "] ";
                break;
            case LogContent::LogLocation:
                ss << "[" << file << ":" << line << " (" << func << ")] ";
                break;
            default:
            case LogContent::Message:
                ss << msg << " ";
                break;
            }
        }
        if (output == Output::Console)
        {
            std::string boldFormatter = config.consoleLog.useBoldText ? "[1;" : "[0;";
            Color logColor = GetLogColor(level, config);
            std::string logColorAnsi = GetLogColorAnsi(logColor);
            // Apply formatting code
            std::string current = ss.str();
            ss.str("");
            ss.clear();
            ss << "\033" << boldFormatter << logColorAnsi << current << "\033[0m";
        }
        return ss.str();
    }
    Color GetLogColor(LogLevel level, Config config)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return config.consoleLog.logColors.debugLogColor;
        case LogLevel::Error:
            return config.consoleLog.logColors.errorLogColor;
        case LogLevel::Fatal:
            return config.consoleLog.logColors.fatalLogColor;
        case LogLevel::Info:
            return config.consoleLog.logColors.infoLogColor;
        case LogLevel::Trace:
            return config.consoleLog.logColors.traceLogColor;
        case LogLevel::Warn:
            return config.consoleLog.logColors.warningLogColor;
        default:
            return Color::White;
        }
    }
    std::string GetLogColorAnsi(Color color)
    {
        switch (color)
        {
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
};

// Convenience macros
#define LOG_TRACE(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Trace, msg, config, __FILE__, __LINE__, __func__)
#define LOG_DEBUG(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Debug, msg, config, __FILE__, __LINE__, __func__)
#define LOG_INFO(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Info, msg, config, __FILE__, __LINE__, __func__)
#define LOG_WARN(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Warn, msg, config, __FILE__, __LINE__, __func__)
#define LOG_ERROR(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Error, msg, config, __FILE__, __LINE__, __func__)
#define LOG_FATAL(msg, config) cpp_flex_logger::Logger::Log(LogLevel::Fatal, msg, config, __FILE__, __LINE__, __func__)

} // namespace cpp_flex_logger

#endif // LOGGER_H

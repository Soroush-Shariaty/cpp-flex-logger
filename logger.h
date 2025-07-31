#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <vector>

// Log levels
enum class LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
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
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
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
    Logger(const Logger &);

    Logger &operator=(const Logger &);

    void log(LogLevel level, const std::string &msg, Config config, const char *file, int line, const char *func)
    {

        if (config.consoleLog.enable)
        {
            std::string formatted = formatMessage(level, msg, config, file, line, func, Output::Console);
            std::lock_guard<std::mutex> lock(mutex_);
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
            std::string formatted = formatMessage(level, msg, config, file, line, func, Output::File);
            logFile << formatted << std::endl;

            logFile.close();
        }
    }
    Logger() {}

  private:
    std::string formatMessage(LogLevel level, const std::string &msg, Config config, const char *file, int line, const char *func, Output output)
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
            Color logColor = getLogColor(level, config);
            std::string logColorAnsi = getLogColorAnsi(logColor);
            // Apply formatting code
            std::string current = ss.str();
            ss.str("");
            ss.clear();
            ss << "\033" << boldFormatter << logColorAnsi << current << "\033[0m";
        }
        return ss.str();
    }
    Color getLogColor(LogLevel level, Config config)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return config.consoleLog.logColors.debugLogColor;
        case LogLevel::ERROR:
            return config.consoleLog.logColors.errorLogColor;
        case LogLevel::FATAL:
            return config.consoleLog.logColors.fatalLogColor;
        case LogLevel::INFO:
            return config.consoleLog.logColors.infoLogColor;
        case LogLevel::TRACE:
            return config.consoleLog.logColors.traceLogColor;
        case LogLevel::WARN:
            return config.consoleLog.logColors.warningLogColor;
        default:
            return Color::White;
        }
    }
    std::string getLogColorAnsi(Color color)
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
    std::mutex mutex_;
    std::once_flag invalidFileFlag;
};

Logger logger;
// Convenience macros
#define LOG_TRACE(msg, config) logger.log(LogLevel::TRACE, msg, config, __FILE__, __LINE__, __func__)
#define LOG_DEBUG(msg, config) logger.log(LogLevel::DEBUG, msg, config, __FILE__, __LINE__, __func__)
#define LOG_INFO(msg, config) logger.log(LogLevel::INFO, msg, config, __FILE__, __LINE__, __func__)
#define LOG_WARN(msg, config) logger.log(LogLevel::WARN, msg, config, __FILE__, __LINE__, __func__)
#define LOG_ERROR(msg, config) logger.log(LogLevel::ERROR, msg, config, __FILE__, __LINE__, __func__)
#define LOG_FATAL(msg, config) logger.log(LogLevel::FATAL, msg, config, __FILE__, __LINE__, __func__)

#endif // LOGGER_H

#include "logger.h"

// Example main
int main() {
    using namespace cpp_flex_logger;
    // Build a config object and fill it with your desired needs
    Config config;
    config.consoleLog.logColors.infoLogColor = Color::Blue;
    config.consoleLog.useBoldText = true;
    config.fileLog.enable = true;
    config.fileLog.absoluteFileLocation = "test.txt";
    config.logContentList = {LogContent::LogLevel, LogContent::TimeStamp, LogContent::LogLocation, LogContent::Message};

    // Log your messages into console and file using your specified options
    LOG_INFO("Starting application...", config);
    LOG_DEBUG("Debugging info", config);
    LOG_WARN("Low memory", config);
    LOG_ERROR("Error occurred!", config);

    // You can also cache the logs for later printing
    for (int var = 0; var < 5; ++var)
    {
        LOG_INFO_QUEUED("Info log queued");
        LOG_DEBUG_QUEUED("Debug log queued");
    }

    // Call this macro frequently to print the queued logs (If you are caching the logs)
    FLUSH_QUEUE(config);

    return 0;
}

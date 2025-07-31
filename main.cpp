#include "logger.h"

// Example main
int main() {

    Config config;
    config.consoleLog.logColors.infoLogColor = Color::Blue;
    config.consoleLog.useBoldText = true;
    config.fileLog.enable = true;
    config.fileLog.absoluteFileLocation = "/opt/test.txt";

    LOG_INFO("Starting application...",config);
    LOG_DEBUG("Debugging info",config);
    LOG_WARN("Warning: Low memory",config);
    LOG_ERROR("Error occurred!",config);

    return 0;
}

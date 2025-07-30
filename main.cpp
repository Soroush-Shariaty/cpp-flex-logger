#include "logger.h"

// Example main
int main() {

    Config config;
    config.logColors.infoLogColor = Color::Blue;

    LOG_INFO("Starting application...",config);
    LOG_DEBUG("Debugging info",config);
    LOG_WARN("Warning: Low memory",config);
    LOG_ERROR("Error occurred!",config);

    return 0;
}

# cpp-flex-logger Library

A lightweight and flexible **header-only** logging library for C++ with support for console and file logging.  
Features include customizable log levels with color support, timestamps, file locations, and more.

## Features

- Header-only: just include the header, no build steps required
- Log messages to console and/or files
- Multiple log levels (DEBUG, INFO, WARN, ERROR, etc.) with user-selectable colors
- Optional timestamps in log entries
- Include file name and line number in logs for easy debugging
- Configurable log formatting
- Easy integration into any C++ project
- Currently only support for gcc conpiler

## Installation

Simply copy the `logger.h` into your project include directory, and include it where needed:

```cpp

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
    LOG_INFO("Starting application...",config);
    LOG_DEBUG("Debugging info",config);
    LOG_WARN("Low memory", config);
    LOG_ERROR("Error occurred!",config);

    return 0;
}
```
## What you get
Colored logs in console

<img width="1091" height="86" alt="cpp-flex-logger-terminal" src="https://github.com/user-attachments/assets/f68b8b38-f488-4c81-8cc9-652ce77fad8a" />

Clean logs listed in file

<img width="939" height="140" alt="cpp-logfile" src="https://github.com/user-attachments/assets/37b455a1-1951-489d-9a12-f59cb07ea644" />

## Contributions

Contributions are welcome! Please open issues or submit pull requests on GitHub after reading the contribution guidelines on CONTRIBUTION.md

## License

This project is licensed under the Apache 2.0 License. See LICENSE for details.

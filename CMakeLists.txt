cmake_minimum_required(VERSION 3.16)

project(cpp-flex-logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp-flex-logger main.cpp
    logger.h)

include(GNUInstallDirs)
install(TARGETS cpp-flex-logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

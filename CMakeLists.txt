cmake_minimum_required(VERSION 3.16)

project(Pro_Logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Pro_Logger main.cpp
    logger.h)

include(GNUInstallDirs)
install(TARGETS Pro_Logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

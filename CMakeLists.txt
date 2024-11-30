cmake_minimum_required(VERSION 3.30)
project(ctm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(Poco REQUIRED COMPONENTS Net NetSSL)
find_package(spdlog CONFIG REQUIRED)

add_executable(
    ctm

    # application main
    src/main.cpp

    # template
    src/template/singleton.hpp
)

target_link_libraries(
    ctm PRIVATE

    Poco::Net
    Poco::NetSSL
    spdlog::spdlog_header_only
)
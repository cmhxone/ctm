cmake_minimum_required(VERSION 3.28)
project(ctm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(Poco REQUIRED COMPONENTS Net NetSSL)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-inih CONFIG REQUIRED)

add_executable(
    ctm

    # application main
    src/main.cpp

    # CTM
    src/ctm/ctm.cpp
    src/ctm/cti_client.cpp
    src/ctm/acceptor/tcp_acceptor.cpp
    src/ctm/acceptor/websocket_acceptor.cpp
    src/ctm/handler/tcp_handler.cpp
    src/ctm/handler/websocket_handler.cpp

    # util
    src/util/ini_loader.cpp
)

target_link_libraries(
    ctm PRIVATE

    Poco::Net
    Poco::NetSSL
    spdlog::spdlog_header_only
    unofficial::inih::inireader
)

# 리소스 파일 이동
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/res)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/res/conf)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/conf/ctm.ini ${CMAKE_CURRENT_BINARY_DIR}/res/conf/ctm.ini COPYONLY)
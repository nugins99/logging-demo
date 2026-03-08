cmake_minimum_required(VERSION 3.20)

project(logging_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(loglib
    log.cpp
)

target_include_directories(loglib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test_logging
    main.cpp
)

target_link_libraries(test_logging
    PRIVATE
        loglib
)

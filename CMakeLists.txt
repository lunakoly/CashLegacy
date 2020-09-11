cmake_minimum_required(VERSION 3.10)

# set the project name
project(Cash VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure platform-specific files
if (WIN32)
    set(PLATFORM_SPECIFIC_FILES windows)
endif (WIN32)

if (UNIX)
    set(PLATFORM_SPECIFIC_FILES unix)
endif (UNIX)

# add the executable
add_executable(
    Main
        sources/main.cpp
        sources/helpers.hpp
        "sources/${PLATFORM_SPECIFIC_FILES}/helpers.hpp"
        sources/terminals/keys.hpp
        sources/terminals/vt100.hpp
        sources/terminals/vt100.cpp
        sources/terminals/vt100_reader.hpp
        sources/terminals/vt100_reader.cpp
        sources/terminals/provider.hpp
        sources/terminals/provider.cpp
        "sources/terminals/${PLATFORM_SPECIFIC_FILES}/features.hpp"
        "sources/terminals/${PLATFORM_SPECIFIC_FILES}/features.cpp"
        sources/processing/tasks.hpp
        sources/processing/launcher.hpp
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/tasks.hpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/process.hpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/process.cpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/input_file.hpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/input_file.cpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/output_file.hpp"
        "sources/processing/${PLATFORM_SPECIFIC_FILES}/output_file.cpp"
        sources/parsing/parser.hpp
        sources/parsing/parser.cpp
)

# accessing common info
configure_file(sources/config.hpp.in sources/config.hpp)

target_include_directories(
    Main
        PUBLIC
            sources/
            "${PROJECT_BINARY_DIR}/sources/"
)

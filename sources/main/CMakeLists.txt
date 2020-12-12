# Copyright (C) 2020 luna_koly


configure_file(
    "about.hpp.in"
    "about.hpp"
)

add_executable(
    Cash
        main.cpp
        helpers.hpp
        "${PLATFORM_SPECIFIC_FILES}/helpers.hpp"
        terminals/keys.hpp
        terminals/vt100.hpp
        terminals/vt100.cpp
        terminals/vt100_reader.hpp
        terminals/vt100_reader.cpp
        terminals/provider.hpp
        terminals/provider.cpp
        "terminals/${PLATFORM_SPECIFIC_FILES}/features.hpp"
        "terminals/${PLATFORM_SPECIFIC_FILES}/features.cpp"
        processing/tasks.hpp
        processing/launcher.hpp
        "processing/${PLATFORM_SPECIFIC_FILES}/tasks.hpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/process.hpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/process.cpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/input_file.hpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/input_file.cpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/output_file.hpp"
        "processing/${PLATFORM_SPECIFIC_FILES}/output_file.cpp"
        parsing/parser.hpp
        parsing/parser.cpp
)

# target_link_libraries(Cash Orders Cringe Threading)

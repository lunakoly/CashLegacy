// Copyright (C) 2020 luna_koly


#include "output_file.hpp"

#include <iostream>

#include "../../windows/helpers.hpp"


std::optional<std::string> WindowsOutputFile::configure(WindowsInputTask * previous) {
    // std::string to char[] because
    // CreateFile requires it
    char * name = new char[filename.length() + 1];
    strcpy_s(name, filename.length() + 1, filename.c_str());

    // non-inheritable handle
    HANDLE output = CreateFile(
        name,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    delete[] name;

    if (output == INVALID_HANDLE_VALUE) {
        return report_error("Output > Invalid file handle for '" + filename + '`');
    }

    // all handles should be uninheritable by default
    if (!SetHandleInformation(output, HANDLE_FLAG_INHERIT, 0)) {
        return report_error("Output > File handle should not be inherited");
    }

    previous->output = output;
    return {};
}


std::optional<std::string> WindowsOutputFile::launch() {
    return {};
}


std::optional<std::string> WindowsOutputFile::wait() {
    return {};
}

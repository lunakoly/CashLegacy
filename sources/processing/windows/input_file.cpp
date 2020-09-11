// Copyright (C) 2020 luna_koly


#include "input_file.hpp"

#include "../../windows/helpers.hpp"


std::optional<std::string> WindowsInputFile::then(WindowsOutputTask * next) {
    this->next = next;

    // std::string to char[] because
    // CreateFile requires it
    char * name = new char[filename.length() + 1];
    strcpy_s(name, filename.length() + 1, filename.c_str());

    // non-inheritable handle
    HANDLE input = CreateFile(
        name,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_READONLY,
        NULL
    );

    delete[] name;

    if (input == INVALID_HANDLE_VALUE) {
        if (GetLastError() == 2) {
            // no `Input > ` because the user
            // may see this error during their normal
            // workflow (it's not an internal error)
            return "Couldn't find the specified file > `" + filename + "`";
        }

        return report_error("Input > Invalid file handle for '" + filename + '`');
    }

    // all handles should be uninheritable by default
    if (!SetHandleInformation(input, HANDLE_FLAG_INHERIT, 0)) {
        return report_error("Input > File handle should not be inherited");
    }

    next->input = input;
    return {};
}


std::optional<std::string> WindowsInputFile::launch() {
    if (next != nullptr) {
        if (auto error = next->launch()) {
            return error;
        }
    }

    return {};
}


std::optional<std::string> WindowsInputFile::wait() {
    if (next != nullptr) {
        return next->wait();
    }

    return {};
}

// Copyright (C) 2020 luna_koly


#include "process.hpp"

#include <windows.h>
#include <sstream>

#include "../../windows/helpers.hpp"


std::optional<std::string> WindowsProcess::then(WindowsOutputTask * next) {
    this->next = next;
    return next->configure(this);
}


std::optional<std::string> WindowsProcess::configure(WindowsInputTask * previous) {
    SECURITY_ATTRIBUTES sequrity_attributes;
    sequrity_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    sequrity_attributes.bInheritHandle = TRUE;
    sequrity_attributes.lpSecurityDescriptor = NULL;

    if (!CreatePipe(
        &(input),
        &(previous->output),
        &sequrity_attributes,
        0
    )) {
        return report_error("Process > Couldn't create a pipe");
    }

    // all handles should be uninheritable by default
    if (!SetHandleInformation(input, HANDLE_FLAG_INHERIT, 0)) {
        return report_error("Process > Input handle should not be inherited");
    }

    if (!SetHandleInformation(previous->output, HANDLE_FLAG_INHERIT, 0)) {
        return report_error("Process > Output handle should not be inherited");
    }

    return {};
}


/**
 * Concatenates arguments into a single
 * command.
 */
static std::string join_arguments(const std::vector<std::string> & arguments) {
    std::stringstream line_stream;

    std::copy(
        arguments.begin(),
        arguments.end(),
        std::ostream_iterator<std::string>(line_stream, " ")
    );

    return line_stream.str();
}


std::optional<std::string> WindowsProcess::launch() {
    if (input == NULL) {
        input = GetStdHandle(STD_INPUT_HANDLE);
    }

    if (next == nullptr) {
        output = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    if (next != nullptr) {
        if (auto error = next->launch()) {
            return error;
        }
    }

    ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));

    STARTUPINFO start_info;
    ZeroMemory(&start_info, sizeof(STARTUPINFO));
    start_info.dwFlags |= STARTF_USESTDHANDLES;
    start_info.cb = sizeof(STARTUPINFO);
    start_info.hStdOutput = output;
    start_info.hStdError = output;
    start_info.hStdInput = input;

    // std::string to char[] because
    // CreateFile requires it
    std::string line = join_arguments(arguments);
    char * command = new char[line.length() + 1];
    strcpy_s(command, line.length() + 1, line.c_str());

    // reenable inheritance for the current IO handles
    if (!SetHandleInformation(input, HANDLE_FLAG_INHERIT, 1)) {
        return report_error("Process > Input handle should be inherited");
    }

    if (!SetHandleInformation(output, HANDLE_FLAG_INHERIT, 1)) {
        return report_error("Process > Output handle should be inherited");
    }

    BOOL success = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &start_info,
        &process_info
    );

    delete[] command;

    if (!success) {
        if (GetLastError() == 2 && !arguments.empty()) {
            // no `Process > ` because the user
            // may see this error during their normal
            // workflow (it's not an internal error)
            return "Couldn't find the specified command > `" + arguments[0] + "`";
        }

        return report_error("Process > Couldn't start a process for `" + std::string(command) + '`');
    }

    if (
        input != GetStdHandle(STD_INPUT_HANDLE) &&
        !CloseHandle(input)
    ) {
        return report_error("Process > Couldn't close the input handle");
    }

    if (
        output != GetStdHandle(STD_OUTPUT_HANDLE) &&
        !CloseHandle(output)
    ) {
        return report_error("Process > Couldn't close the output handle");
    }

    return {};
}


std::optional<std::string> WindowsProcess::wait() {
    // blocks until the process terminates
    WaitForSingleObject(process_info.hProcess, INFINITE);
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    if (next != nullptr) {
        return next->wait();
    }

    return {};
}

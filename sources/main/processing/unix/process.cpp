// Copyright (C) 2020 luna_koly


#include "process.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <iterator>
#include <sstream>
#include <cstring>
#include <iostream>

#include "../../unix/helpers.hpp"
#include "../../helpers.hpp"


std::optional<std::string> UnixProcess::then(UnixOutputTask * next) {
    this->next = next;
    next->previous = this;
    return next->configure(this);
}


std::optional<std::string> UnixProcess::configure(UnixInputTask * previous) {
    int fd[2];
    pipe(fd);

    previous->output = fd[1];
    input = fd[0];

    return {};
}


std::optional<std::string> UnixProcess::launch() {
    if (input == 0) {
        input = fileno(stdin);
    }

    if (next == nullptr) {
        output = fileno(stdout);
    }

    if (next != nullptr) {
        if (auto error = next->launch()) {
            return error;
        }
    }

    int child_id = fork();

    if (child_id == -1) {
        return report_error("Process > Couldn't start a process for `" + join(arguments) + '`');
    }

    const char * command = arguments[0].c_str();

    char ** rest = new char * [arguments.size() + 1];
    rest[arguments.size()] = NULL;

    for (size_t it = 0; it < arguments.size(); it++) {
        rest[it] = new char [arguments[it].length() + 1];
        rest[it][arguments[it].length()] = '\0';
        arguments[it].copy(rest[it], arguments[it].length(), 0);
    }

    if (child_id == 0) {
        dup2(output, fileno(stdout));
        dup2(input, fileno(stdin));

        // we've created a child, but it also
        // has all file descriptors of all other pipes
        // because we can't disable their inheritance temporarily
        if (previous != nullptr) {
            previous->close_all_previous();
        }

        if (next != nullptr) {
            next->close_all_next();
        }

        int result = execvp(command, rest);
        // this code is executed in case of an
        // error (result == -1) only.

        if (errno == EACCES && !arguments.empty()) {
            // no `Process > ` because the user
            // may see this error during their normal
            // workflow (it's not an internal error)
            std::cout << "Couldn't find the specified command > `" + arguments[0] + "`" << std::endl;
        } else {
            std::cout << report_error("Process > Couldn't start a process for `" + std::string(command) + '`') << std::endl;
        }

        exit(0);
    }

    // parent doesn't need any of these
    if (input != fileno(stdin)) {
        close(input);
    }

    if (output != fileno(stdout)) {
        close(output);
    }

    return {};
}


std::optional<std::string> UnixProcess::wait() {
    int status = 0;
    int child_pid = 0;

    while ((child_pid = ::wait(&status)) > 0) {
        usleep(16000);
    }

    // in fact, we don't need to go
    // with next->wait(), but I left it
    // here just for consistency
    if (next != nullptr) {
        return next->wait();
    }

    return {};
}


void UnixProcess::close_all_next() {
    if (input != fileno(stdin)) {
        close(input);
    }

    if (output != fileno(stdout)) {
        close(output);
    }

    if (next != nullptr) {
        next->close_all_next();
    }
}


void UnixProcess::close_all_previous() {
    if (input != fileno(stdin)) {
        close(input);
    }

    if (output != fileno(stdout)) {
        close(output);
    }

    if (previous != nullptr) {
        previous->close_all_previous();
    }
}

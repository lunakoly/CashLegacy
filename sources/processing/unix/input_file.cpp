// Copyright (C) 2020 luna_koly


#include "input_file.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../unix/helpers.hpp"


std::optional<std::string> UnixInputFile::then(UnixOutputTask * next) {
    this->next = next;
    next->previous = this;

    // 0666 - permissions
    int input = open(filename.c_str(), O_RDONLY, 0666);

    if (input == -1) {
        if (errno == ENOENT) {
            // no `Input > ` because the user
            // may see this error during their normal
            // workflow (it's not an internal error)
            return "Couldn't find the specified file > `" + filename + "`";
        }

        return report_error("Input > Invalid file handle for '" + filename + '`');
    }

    next->input = input;
    return {};
}


std::optional<std::string> UnixInputFile::launch() {
    if (next != nullptr) {
        if (auto error = next->launch()) {
            return error;
        }
    }

    return {};
}


std::optional<std::string> UnixInputFile::wait() {
    if (next != nullptr) {
        return next->wait();
    }

    return {};
}


void UnixInputFile::close_all_next() {
    if (next != nullptr) {
        next->close_all_next();
    }
}


void UnixInputFile::close_all_previous() {
    // chill
}

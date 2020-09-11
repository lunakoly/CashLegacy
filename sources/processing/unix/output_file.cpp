// Copyright (C) 2020 luna_koly


#include "output_file.hpp"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


std::optional<std::string> UnixOutputFile::configure(UnixInputTask * previous) {
    // 0666 - permissions
    // O_TRUNC - override the file
    previous->output = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    return {};
}


std::optional<std::string> UnixOutputFile::launch() {
    return {};
}


std::optional<std::string> UnixOutputFile::wait() {
    return {};
}


void UnixOutputFile::close_all_next() {
    // chill
}


void UnixOutputFile::close_all_previous() {
    if (previous != nullptr) {
        previous->close_all_previous();
    }
}

// Copyright (C) 2020 luna_koly
//
// Parses terminal input.


#pragma once


#include <string>
#include <vector>
#include <optional>


/**
 * Represents the total action
 * the user has requested.
 */
struct Request {
    std::string input;
    std::string output;
    std::vector<std::vector<std::string>> commands;
};


namespace Parser {
    /**
     * Parses the line into a list of commands
     * to be run in a pipeline.
     */
    std::optional<Request> parse(const std::string & line);
}

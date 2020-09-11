// Copyright (C) 2020 luna_koly


#include "parser.hpp"

#include <sstream>
#include <iostream>


static bool is_blank(char it) {
    return it == '\t' ||
           it == '\n' ||
           it == ' ';
}


static void skip_blank(std::istream & input) {
    while (input.peek() != EOF && is_blank(input.peek())) {
        input.get();
    }
}


static void read_quoted(std::istream & input, std::ostream & output) {
    // get the quote
    char quote = input.get();

    while (
        input.peek() != EOF &&
        input.peek() != quote
    ) {
        char it = input.get();

        if (it == '\\') {
            it = input.get();
        }

        output << it;
    }
}


static bool is_delimiter(char it) {
    return it == '|' ||
           it == '<' ||
           it == '>';
}


std::string read_word(std::istream & input) {
    skip_blank(input);
    std::stringstream output;

    while (
        input.peek() != EOF &&
        !is_blank(input.peek()) &&
        !is_delimiter(input.peek())
    ) {
        if (input.peek() == '"' || input.peek() == '\'') {
            read_quoted(input, output);
        } else {
            char it = input.get();

            if (it == '\\') {
                it = input.get();
            }

            output << it;
        }
    }

    return output.str();
}


std::optional<std::string> parse(Request & request, std::istream & input) {
    while (input.peek() != EOF) {
        skip_blank(input);

        if (input.peek() == '|') {
            if (request.commands.empty()) {
                return "Error > We haven't implemented cyclic pipes yet :)";
            }

            if (request.commands.back().empty()) {
                return "Error > 2 flow operators near each other is not allowed.";
            }

            input.get();
            request.commands.emplace_back();
        } else if (input.peek() == '<') {
            if (request.commands.empty()) {
                return "Error > Unexpected `<`";
            }

            if (request.commands.back().empty()) {
                return "Error > 2 flow operators near each other is not allowed.";
            }

            if (!request.input.empty()) {
                return "Error > You've already specified an input file!";
            }

            input.get();
            request.input = read_word(input);
        } else if (input.peek() == '>') {
            if (request.commands.empty()) {
                return "Error > Unexpected `>`";
            }

            if (request.commands.back().empty()) {
                return "Error > 2 flow operators near each other is not allowed.";
            }

            if (!request.output.empty()) {
                return "Error > You've already specified an output file!";
            }

            input.get();
            request.output = read_word(input);
        } else {
            if (request.commands.empty()) {
                request.commands.emplace_back();
            }

            auto & command = request.commands.back();
            command.emplace_back(read_word(input));
        }
    }

    if (
        request.commands.empty() &&
        (!request.input.empty() || !request.output.empty())
    ) {
        return "Error > You need a command, don't you?";
    }

    if (
        !request.commands.empty() &&
        request.commands.back().empty()
    ) {
        return "Error > You need a command, don't you?";
    }

    return {};
}


std::optional<Request> Parser::parse(const std::string & line) {
    std::stringstream stream;
    Request request;

    stream << line;

    if (auto error = parse(request, stream)) {
        std::cout << error.value() << std::endl;
        return {};
    }

    return request;
}

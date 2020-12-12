// Copyright (C) 2020 luna_koly
//
// This is just the main file.


#include <iostream>
#include <iterator>

#include "about.hpp"
#include "terminals/provider.hpp"
#include "processing/launcher.hpp"
#include "parsing/parser.hpp"


/**
 * DEBUG:
 * Prints the current platform and it's
 * capabilities.
 */
static void print_platform() {
    #ifdef _WIN32
        std::cout << "Running on [Windows]" << std::endl;
    #endif

    #ifdef __unix__
        std::cout << "Running on [Unix-like]" << std::endl;
    #endif

    #ifdef __linux__
        std::cout << "Running on [Linux]" << std::endl;
    #endif

    #ifdef __APPLE__
        std::cout << "Running on [macOS]" << std::endl;
    #endif
}


/**
 * DEBUG:
 * Just for tests.
 */
static int test() {
    print_platform();

    // auto & terminal = TerminalProvider::provide();

    // if (!terminal.is_ok()) {
    //     std::cout << "Error > Couldn't initialize the terminal > " << terminal.get_error() << std::endl;
    //     return 1;
    // }

    // std::cout << "> ";
    // auto line = terminal.read_line();
    // std::cout << std::endl;

    // std::cout << "You said: `" << line << "`" << std::endl;

    // auto result = Parser::parse(line);

    // if (!result) {
    //     return 1;
    // }

    // auto request = result.value();
    // Launcher * launcher = nullptr;

    // if (!request.input.empty()) {
    //     launcher = new Launcher{new InputFile{request.input}};
    //     auto & reference = launcher->then(new Process{request.commands[0]});
    //     launcher = &reference;
    // } else {
    //     launcher = new Launcher{new Process{request.commands[0]}};
    // }

    // for (size_t it = 1; it < request.commands.size(); it++) {
    //     launcher->then(new Process{request.commands[it]});
    // }

    // if (!request.output.empty()) {
    //     launcher->then(new OutputFile{request.output});
    // }

    // auto error = launcher->launch().wait();



    // auto error = Processing::start(new Process({"cmd", "/c", "notepad.exe"}))
    //                          .launch().wait();

    // auto error = Processing::start(new Process({"cmd", "/c", "echo test"}))
    //                          .then(new OutputFile{"../tests/b.txt"})
    //                          .launch().wait();

    // auto error = Processing::start(new InputFile{"../tests/a.txt"})
    //                          .launch().wait();

    // auto error = Processing::start(new InputFile{"../tests/a.txt"})
    //                          .then(new OutputFile{"../tests/b.txt"})
    //                          .launch().wait();

    // auto error = Processing::start(new InputFile{"../tests/a.txt"})
    //                          .then(new Process({"../tests/wrap"}))
    //                          .launch().wait();

    // auto error = Processing::start(new Process({"../tests/wrap"}))
    //                          .then(new Process({"../tests/hide"}))
    //                          .launch().wait();

    // auto error = Processing::start(new InputFile{"../tests/a.txt"})
    //                          .then(new Process({"../tests/wrap"}))
    //                          .then(new Process({"../tests/hide"}))
    //                          .then(new OutputFile{"../tests/b.txt"})
    //                          .launch().wait();

    // auto error = Processing::start(new Process({"echo", "test", "fest"}))
    //                          .launch().wait();

    auto error = Processing::start(new InputFile{"dawf"})
                             .then(new Process({"gawd"}))
                             .launch().wait();

    if (error) {
        std::cout << "Error > " << error.value() << std::endl;
    }

    return 0;
}


/**
 * The main loop.
 * I put it here separately to simplify
 * switching to test().
 */
static int loop() {
    auto & terminal = TerminalProvider::provide();

    while (true) {
        if (!terminal.is_ok()) {
            std::cout << "Error > " << terminal.get_error() << std::endl;
            return 1;
        }

        std::cout << "=> ";
        auto line = terminal.read_line();
        std::cout << std::endl;

        auto result = Parser::parse(line);

        if (!result) {
            continue;
        }

        auto request = result.value();

        if (
            !request.commands.empty() &&
            !request.commands.back().empty() &&
            request.commands.back().front() == "exit"
        ) {
            return 0;
        }

        Launcher * launcher = nullptr;

        if (!request.input.empty()) {
            launcher = new Launcher{new InputFile{request.input}};
            launcher->then(new Process{request.commands[0]});
        } else {
            launcher = new Launcher{new Process{request.commands[0]}};
        }

        for (size_t it = 1; it < request.commands.size(); it++) {
            launcher->then(new Process{request.commands[it]});
        }

        if (!request.output.empty()) {
            launcher->then(new OutputFile{request.output});
        }

        auto error = launcher->launch().wait();
        delete launcher;

        if (error) {
            std::cout << "Error > " << error.value() << std::endl;
        }
    }

    return 0;
}


int main(int argc, char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    // std::cout.setf(std::ios::unitbuf);

    // return test();
    return loop();
}

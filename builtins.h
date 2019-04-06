#pragma once

#include <iostream>

#include "arguments.h"
#include "state.h"
#include "platform.h"


/**
	Various builtin commands
*/
struct Builtins {
	/**
		Assigns values to variables
	*/
	static void define(State & state, const Arguments & args) {
		if (args.size() >= 3)
			state.define(*args[1], *args[2]);
		else if (args.size() == 2)
			state.erase(*args[1]);
	}

	/**
		Writes data to stdout
	*/
	static void print(const Arguments & args, std::ostream & output) {
		if (args.size() > 1) {
			output << *args[1];

			for (size_t it = 2; it < args.size(); it++)
				output << ' ' << *args[it];
		}
	}

	/**
		Writes data to stdout and
		appends newline character
	*/
	static void echo(const Arguments & args, std::ostream & output) {
		print(args, output);
		output << '\n';
	}

	/**
		Termination current evaluation
	*/
	static void exit(State & state) {
		state.shouldExit = true;
	}

	/**
		Prints the current directory
	*/
	static void cwd(std::ostream & output) {
		output << Platform::getCurrentDirectory();
	}
};
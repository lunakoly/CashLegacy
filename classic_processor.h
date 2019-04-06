#pragma once

#include "processor.h"

#include <iostream>
#include <sstream>
#include <string>

#include "state.h"
#include "arguments.h"
#include "builtins.h"
#include "parser.h"


/**
	Manages command execution
*/
struct ClassicProcessor : public Processor {
	Parser * parser = nullptr;

	/**
		Executes the given command
	*/
	virtual void execute(
		State & state,
		const Arguments & args,
		std::ostream & output
	) {
		if (args.size() > 0) {
			if (*args[0] == "def")
				Builtins::define(state, args);

			else if (*args[0] == "exit")
				Builtins::exit(state);

			else if (*args[0] == "print")
				Builtins::print(args, output);

			else if (*args[0] == "echo")
				Builtins::echo(args, output);

			else if (*args[0] == "cwd")
				Builtins::cwd(output);

			else if (state.contains(*args[0])) {
				std::stringstream value = std::stringstream(state[*args[0]]);
				State inner = State({}, &state);

				for (auto it = 1; it < args.size(); it++)
					inner.define("$" + std::to_string(it), *args[it]);

				parser->parseAll(inner, value, output);
			}

			else {
				output << "Error > Command `" << *args[0] << "` not found\n";
			}
		}
	}
};
#pragma once

#include "processor.h"

#include <memory>

#include <iostream>
#include <sstream>
#include <string>

#include "state.h"
#include "values/value.h"
#include "values/string_value.h"

#include "parser.h"

#include "builtins/general.h"
#include "builtins/values.h"
#include "builtins/display.h"
#include "builtins/shorthands.h"
#include "builtins/numeric.h"


/**
	Manages command execution
*/
struct ClassicProcessor : public Processor {
	Parser * parser = nullptr;

	/**
		Executes the given command
	*/
	virtual std::shared_ptr<Value> execute(
		State & state,
		const Arguments & args,
		std::ostream & output
	) override {
		std::stringstream result;

		if (args.size() > 0) {
			std::string command = args[0]->toString();

			if (command == "typeof")
				Builtins::typeof(args, result);

			else if (command == "exit")
				Builtins::exit(state, args);

			else if (command == "eval")
				Builtins::eval(state, args, *parser, output);

			else if (command == "def")
				Builtins::def(state, args);

			else if (command == "set")
				Builtins::set(state, args);

			else if (command == "exec")
				return Builtins::exec(state, args, *parser, output);

			else if (command == "print")
				Builtins::print(args, result);

			else if (command == "echo")
				Builtins::echo(args, result);

			else if (command == "pwd")
				Builtins::pwd(result);

			else if (command == "username")
				Builtins::username(result);

			else if (command == "hostname")
				Builtins::hostname(result);

			else if (command == "int")
				return Builtins::toInt(args);

			else if (command == "float")
				return Builtins::toFloat(args);

			else if (state.contains(command))
				return state.get(command);

			else
				output << "Error > Command `" << command << "` not found\n";
		}

		return std::make_shared<StringValue>(result.str());
	}
};
#pragma once

#include "processor.h"

#include <memory>

#include <iostream>
#include <sstream>
#include <string>

#include "state.h"
#include "values/value.h"
#include "values/string_value.h"

#include "arguments.h"
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
		if (args.size() > 0) {
			std::stringstream result;
			std::string command = args[0]->toString();

			if (command == "def")
				Builtins::def(state, args);

			else if (command == "set")
				Builtins::set(state, args);

			else if (command == "exit")
				Builtins::exit(state, args);

			else if (command == "print")
				Builtins::print(args, result);

			else if (command == "echo")
				Builtins::echo(args, result);

			else if (command == "typeof")
				Builtins::typeof(args, result);

			else if (command == "int")
				return Builtins::toInt(args);

			else if (command == "float")
				return Builtins::toFloat(args);

			else if (command == "pwd")
				Builtins::pwd(result);

			else if (command == "pun")
				Builtins::pun(result);

			else if (command == "phn")
				Builtins::phn(result);

			else if (command == "exec") {
				std::shared_ptr<Value> contents = state.get(args[1]->toString());
				std::stringstream value = std::stringstream(contents->toString());
				State inner = State({}, &state);

				for (auto it = 2; it < args.size(); it++)
					inner.define("@" + std::to_string(it - 1), args[it]);

				parser->parseAll(inner, value, output);
				return inner.returnValue;
			}

			else if (state.contains(command))
				return state.get(command);

			else {
				output << "Error > Command `" << command << "` not found\n";
			}

			return std::make_shared<StringValue>(result.str());
		}

		return std::make_shared<StringValue>("");
	}
};
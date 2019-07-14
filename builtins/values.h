#pragma once

#include <iostream>

#include "../state.h"
#include "../values/value.h"
#include "../values/string_value.h"
#include "../values/int_value.h"

#include "../parser.h"


/**
	General value & state management
*/
namespace Builtins {
	/**
		Prints the type of a value
	*/
	void typeof(const Arguments & args, std::ostream & output) {
		if (args.size() >= 2)
			output << args[1]->getType();
	}

	/**
		Assigns values to local variables
	*/
	void def(State & state, const Arguments & args) {
		if (args.size() >= 3)
			state.define(args[1]->toString(), args[2]);
	}

	/**
		Assigns values to already defined variables
	*/
	void set(State & state, const Arguments & args) {
		if (args.size() >= 3)
			state.set(args[1]->toString(), args[2]);
	}

	/**
		Evaluates the first agrument
	*/
	void eval(
		State & state,
		const Arguments & args,
		Parser & parser,
		std::ostream & output
	) {
		if (args.size() >= 2) {
			std::string contents = args[1]->toString();
			auto input = std::stringstream(contents);
			parser.parseAll(state, input, output);
		}
	}

	/**
		Evaluates the value of the first agrument and binds
		the following ones as its own arguments
	*/
	std::shared_ptr<Value> exec(
		State & state,
		const Arguments & args,
		Parser & parser,
		std::ostream & output
	) {
		if (args.size() >= 2) {
			auto target = state.get(args[1]->toString());
			std::string contents("");

			if (target->getType() == "code")
				contents = *((std::string *) target->extract());
			else
				contents = target->toString();

			auto input = std::stringstream(contents);
			State inner = State({}, &state);

			for (auto it = 2; it < args.size(); it++)
				inner.define("@" + std::to_string(it - 1), args[it]);

			inner.define("@COUNT", std::make_shared<IntValue>(args.size() - 2));
			parser.parseAll(inner, input, output);
			return inner.returnValue;
		}

		return std::make_shared<StringValue>("");
	}
}
#pragma once

#include <iostream>

#include "../state.h"
#include "../values/value.h"

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
		Evaluates the value of the first agrument and binds
		the following ones as its own arguments
	*/
	std::shared_ptr<Value> exec(
		State & state,
		const Arguments & args,
		Parser & parser,
		std::ostream & output
	) {
		auto contents = state.get(args[1]->toString());
		auto value = std::stringstream(contents->toString());
		State inner = State({}, &state);

		for (auto it = 2; it < args.size(); it++)
			inner.define("@" + std::to_string(it - 1), args[it]);

		parser.parseAll(inner, value, output);
		return inner.returnValue;
	}
}
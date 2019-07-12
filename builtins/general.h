#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "../state.h"
#include "../values/value.h"

#include "../parser.h"


/**
	Most low-level builtin commands
	Cash can't exist without
*/
namespace Builtins {
	/**
		Termination current evaluation
	*/
	void exit(State & state, const Arguments & args) {
		state.shouldExit = true;

		if (args.size() >= 2)
			state.returnValue = args[1];
	}

	/**
		Evaluates the first agrument and binds
		the following ones as its own arguments
	*/
	std::shared_ptr<Value> eval(
		State & state,
		const Arguments & args,
		Parser & parser,
		std::ostream & output
	) {
		auto value = std::stringstream(args[1]->toString());
		State inner = State({}, &state);

		for (auto it = 2; it < args.size(); it++)
			inner.define("@" + std::to_string(it - 1), args[it]);

		parser.parseAll(inner, value, output);
		return inner.returnValue;
	}
}
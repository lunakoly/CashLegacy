#pragma once

#include <iostream>

#include "../state.h"
#include "../arguments.h"


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
}
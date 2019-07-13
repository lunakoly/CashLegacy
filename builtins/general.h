#pragma once

#include "../state.h"
#include "../values/value.h"


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
}
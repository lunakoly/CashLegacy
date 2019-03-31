#pragma once

#include <iostream>

#include "arguments.h"
#include "state.h"


/**
	Can both start OS-dependent processes,
	execute builtin commands and execute
	variable-functions
*/
struct Processor {
	/**
		Executes builtin or function or
		starts new process
	*/
	virtual void execute(
		State & state,
		const Arguments & args,
		std::ostream & output
	) = 0;
};
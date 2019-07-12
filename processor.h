#pragma once

#include <memory>
#include <iostream>

#include "state.h"
#include "values/value.h"


/**
	Can both start OS-dependent processes,
	execute builtin commands and execute
	variable-functions
*/
struct Processor {
	~Processor() {}

	/**
		Executes builtin or function or
		starts new process
	*/
	virtual std::shared_ptr<Value> execute(
		State & state,
		const Arguments & args,
		std::ostream & output
	) = 0;
};
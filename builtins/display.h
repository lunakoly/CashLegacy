#pragma once

#include <iostream>

#include "../values/value.h"


/**
	Most required commands for printing text.
	Added to builtins for opimization purposes.
*/
namespace Builtins {
	/**
		Writes data to stdout
	*/
	void print(const Arguments & args, std::ostream & output) {
		if (args.size() > 1) {
			output << args[1]->toString();

			for (size_t it = 2; it < args.size(); it++)
				output << ' ' << args[it]->toString();
		}
	}

	/**
		Writes data to stdout and
		appends newline character
	*/
	void echo(const Arguments & args, std::ostream & output) {
		print(args, output);
		output << '\n';
	}
}
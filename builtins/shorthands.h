#pragma once

#include <iostream>

#include "../platforms/platform.h"


/**
	Commands that simplify something.
	Added to builtins for opimization purposes.
*/
namespace Builtins {
	/**
		Prints the current directory
	*/
	void pwd(std::ostream & output) {
		output << Platform::getCurrentDirectory();
	}

	/**
		Prints user name
	*/
	void username(std::ostream & output) {
		output << Platform::getUserName();
	}

	/**
		Prints host name
	*/
	void hostname(std::ostream & output) {
		output << Platform::getHostName();
	}
}
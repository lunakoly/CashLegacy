#pragma once

#include <memory>
#include <iostream>

#include "value.h"
#include "float_value.h"
#include "integer_value.h"

#include "state.h"
#include "arguments.h"
#include "platform.h"


/**
	Various builtin commands
*/
struct Builtins {
	/**
		Assigns values to local variables
	*/
	static void define(State & state, const Arguments & args) {
		if (args.size() >= 3)
			state.define(args[1]->toString(), args[2]);
	}

	/**
		Assigns values to already defined variables
	*/
	static void set(State & state, const Arguments & args) {
		if (args.size() >= 3)
			state.set(args[1]->toString(), args[2]);
	}

	/**
		Writes data to stdout
	*/
	static void print(const Arguments & args, std::ostream & output) {
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
	static void echo(const Arguments & args, std::ostream & output) {
		print(args, output);
		output << std::endl;
	}

	/**
		Prints the type of a value
	*/
	static void typeOf(const Arguments & args, std::ostream & output) {
		if (args.size() >= 2)
			output << args[1]->getType();
	}

	/**
		Turns a value into an int
	*/
	static std::shared_ptr<IntegerValue> toInt(const Arguments & args) {
		if (args.size() >= 2) {
			if (args[1]->getType() == "string") {
				auto value = (std::string *) args[1]->extract();
				return std::make_shared<IntegerValue>(std::stoi(*value));
			}

			if (args[1]->getType() == "float") {
				auto value = (float *) args[1]->extract();
				return std::make_shared<IntegerValue>(*value);
			}
		}

		return std::make_shared<IntegerValue>(0);
	}

	/**
		Turns a value into a float
	*/
	static std::shared_ptr<FloatValue> toFloat(const Arguments & args) {
		if (args.size() >= 2) {
			if (args[1]->getType() == "string") {
				auto value = (std::string *) args[1]->extract();
				return std::make_shared<FloatValue>(std::stof(*value));
			}

			if (args[1]->getType() == "integer") {
				auto value = (int *) args[1]->extract();
				return std::make_shared<FloatValue>(*value);
			}
		}

		return std::make_shared<FloatValue>(0.0);
	}

	/**
		Termination current evaluation
	*/
	static void exit(State & state, const Arguments & args) {
		state.shouldExit = true;

		if (args.size() >= 2)
			state.returnValue = args[1];
	}

	/**
		Prints the current directory
	*/
	static void pwd(std::ostream & output) {
		output << Platform::getCurrentDirectory();
	}

	/**
		Prints user name
	*/
	static void pun(std::ostream & output) {
		output << Platform::getUserName();
	}

	/**
		Prints host name
	*/
	static void phn(std::ostream & output) {
		output << Platform::getHostName();
	}
};
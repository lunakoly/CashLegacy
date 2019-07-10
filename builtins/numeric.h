#pragma once

#include <memory>
#include <string>

#include "../values/integer_value.h"
#include "../values/float_value.h"

#include "../state.h"
#include "../arguments.h"


/**
	Value optimizations for numeric calculations
*/
namespace Builtins {
	/**
		Turns a value into an int
	*/
	std::shared_ptr<IntegerValue> toInt(const Arguments & args) {
		try {
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
		} catch (const std::exception & e) {}

		return std::make_shared<IntegerValue>(0);
	}

	/**
		Turns a value into a float
	*/
	std::shared_ptr<FloatValue> toFloat(const Arguments & args) {
		try {
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
		} catch (const std::exception & e) {}

		return std::make_shared<FloatValue>(0.0);
	}
}
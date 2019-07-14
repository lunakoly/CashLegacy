#pragma once

#include "value.h"

#include <string>


/**
	A value that stores an int
	internally and thus is designed
	to be used as an int
*/
class IntValue : public Value {
private:
	int value;

public:
	IntValue(int value) : value(value) {}

	/**
		Because Cash is a command-line language
		any value must have a string representation
	*/
	virtual std::string toString() const override {
		return std::to_string(value);
	}

	/**
		Allows to find out the optimization type.
		Added mostly for debugging
	*/
	virtual std::string getType() const override {
		return "int";
	}

	/**
		Returns pointer to the internal optimized
		value
	*/
	virtual void * extract() override {
		return &value;
	}
};
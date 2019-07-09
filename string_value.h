#pragma once

#include <string>


/**
	A value that stores a string
	internally and thus is designed
	to be used as a string
*/
class StringValue : public Value {
private:
	std::string value;

public:
	StringValue(const std::string & value) : value(value) {}
	StringValue(std::string && value) : value(std::move(value)) {}

	/**
		Because Cash is a command-line language
		any value must have a string representation
	*/
	virtual std::string toString() const override {
		return value;
	}

	/**
		Allows to find out the optimization type.
		Added mostly for debugging
	*/
	virtual std::string getType() const {
		return "string";
	}

	/**
		Returns pointer to the internal optimized
		value
	*/
	virtual void * extract() override {
		return &value;
	}
};
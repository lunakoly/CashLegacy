#pragma once

#include <string>


/**
	A value that stores an float
	internally and thus is designed
	to be used as a float
*/
class FloatValue : public Value {
private:
	float value;

public:
	FloatValue(float value) : value(value) {}

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
	virtual std::string getType() const {
		return "float";
	}

	/**
		Returns pointer to the internal optimized
		value
	*/
	virtual void * extract() override {
		return &value;
	}
};
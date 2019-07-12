#pragma once

#include <memory>

#include <string>
#include <vector>


/**
	Represents anything starting from
	command name and arguments and up to
	function results
*/
struct Value {
	virtual ~Value() {}

	/**
		Because Cash is a command-line language
		any value must have a string representation
	*/
	virtual std::string toString() const = 0;

	/**
		Allows to find out the optimization type.
		Added mostly for debugging
	*/
	virtual std::string getType() const = 0;

	/**
		Returns pointer to the internal optimized
		value
	*/
	virtual void * extract() = 0;
};

/**
	Shortcut for arguments list
*/
using Arguments = std::vector<std::shared_ptr<Value>>;

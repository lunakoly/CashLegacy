#pragma once

#include <unordered_map>
#include <string>


/**
	Holds important for the workflow parameters,
	variables and supports hierarchy
*/
class State {
private:
	std::unordered_map<std::string, std::string> variables;
	State * parent;

public:
	/**
		Can be used to mark end of
		the workflow
	*/
	bool shouldExit = false;

	State(
		std::unordered_map<std::string, std::string> variables = {},
		State * parent = nullptr
	) : variables(variables), parent(parent) {}

	/**
		Searches for the value recursively
	*/
	std::string & operator [] (const std::string & key) {
		auto it = variables.find(key);

		// ask parent scope if the requested
		// was not found
		if (it == variables.end() && parent != nullptr)
			return (*parent)[key];

		return variables[key];
	}

	/**
		Totally removes the entry
	*/
	void erase(const std::string & key) {
		variables.erase(key);
	}

	/**
		Returns true if the requested entry
		is presented
	*/
	bool contains(const std::string & key) const {
		bool isDeclared = variables.find(key) != variables.end();

		if (!isDeclared && parent != nullptr)
			return parent->contains(key);

		return isDeclared;
	}

	/**
		Adds a local variable
	*/
	void define(const std::string & key, std::string value) {
		variables[key] = value;
	}
};
#pragma once

#include <memory>

#include <unordered_map>
#include <string>

#include "value.h"
#include "string_value.h"


/**
	Holds important for the workflow parameters,
	variables and supports hierarchy
*/
class State {
private:
	std::unordered_map<std::string, std::shared_ptr<Value>> variables;
	State * parent;

public:
	/**
		Can be used to mark end of
		the workflow
	*/
	bool shouldExit = false;

	/**
		Function return value.
		Empty string by default
	*/
	std::shared_ptr<Value> returnValue = std::make_shared<StringValue>("");

	State(
		std::unordered_map<std::string, std::shared_ptr<Value>> variables = {},
		State * parent = nullptr
	) : variables(variables), parent(parent) {}

	/**
		Adds a local variable
	*/
	void define(const std::string & key, const std::shared_ptr<Value> & value) {
		variables[key] = value;
	}

	/**
		Totally removes the entry
	*/
	void erase(const std::string & key) {
		variables.erase(key);
	}

	/**
		Tries to change the value of the
		existing variable. Returns false on fail
	*/
	bool set(const std::string & key, const std::shared_ptr<Value> & value) {
		auto it = variables.find(key);

		if (it != variables.end()) {
			it->second = value;
			return true;
		}

		return false;
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
		Returns the variable value or an empty
		string value
	*/
	std::shared_ptr<Value> get(const std::string & key) const {
		auto it = variables.find(key);

		if (it != variables.end())
			return it->second;

		if (parent != nullptr)
			return parent->get(key);

		return std::make_shared<StringValue>("");
	}
};
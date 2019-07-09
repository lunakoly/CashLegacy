#pragma once

#include <memory>

#include <string>
#include <sstream>

#include <forward_list>
#include <vector>

#include "value.h"


/**
	Shortcut for primary arguments storage type
*/
using Arguments = std::vector<std::shared_ptr<Value>>;

/**
	Shortcut for temporary arguments storage.
	Initially arguments are stored inside of a
	linked list and then moved into a vector
*/
class ArgumentsCollector {
private:
	std::forward_list<std::shared_ptr<Value>> temporary;
	size_t count = 0;

public:
	/**
		Adds strings to inner temporary storage
	*/
	void add(std::shared_ptr<Value> & value) {
		temporary.push_front(value);
		count++;
	}

	/**
		Converts inner temporary storage
		to Arguments
	*/
	const Arguments collect() {
		Arguments args = Arguments(count);

		auto it = temporary.begin();
		size_t that = count - 1;

		while (
			it != temporary.end() &&
			that >= 0
		) {
			args[that] = std::move(*it);
			that--;
			it++;
		}

		return args;
	}
};
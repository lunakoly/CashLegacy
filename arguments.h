#pragma once

#include <memory>

#include <string>
#include <sstream>

#include <forward_list>
#include <vector>


// When working with command/function arguments
// they are read to stringstreams, converted
// to strings and stored as unique_ptrs


/**
	Shortcut for primary arguments storage type
*/
using Arguments = std::vector<std::unique_ptr<std::string>>;

/**
	Shortcut for temporary arguments storage.
	Linked list is easier to fill
	compared to vector
*/
class ArgumentsCollector {
private:
	std::forward_list<std::unique_ptr<std::string>> temporary;
	size_t count = 0;

public:
	/**
		Adds strings to inner temporary storage
	*/
	void add(std::string value) {
		temporary.push_front(std::make_unique<std::string>(value));
		count++;
	}

	/**
		Converts inner temporary storage
		to Arguments
	*/
	const Arguments collect() {
		Arguments args = Arguments(count);

		auto it = temporary.begin();
		int that = count - 1;

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
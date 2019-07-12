#pragma once

#include <memory>

#include <iostream>
#include <string>
#include <sstream>

#include <forward_list>
#include <vector>

#include "values/value.h"
#include "values/string_value.h"


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
	/**
		Stores the value and the flag that determines
		if there was a whitespace after it
		so that this value should not
		be clued together with the next one
	*/
	struct Node {
		const std::shared_ptr<Value> value;
		bool isSeam;
	};

	std::forward_list<Node> temporary;
	Node * lastAddedNode = nullptr;

public:
	/**
		Adds strings to inner temporary storage
	*/
	void add(const std::shared_ptr<Value> & value) {
		temporary.push_front({ value, false });
		lastAddedNode = &(*(temporary.begin()));
	}

	/**
		Defines a seam between values meaning
		that the last added value should not be
		clued with the next one being added
	*/
	void markSeam() {
		if (lastAddedNode != nullptr)
			lastAddedNode->isSeam = true;
	}

	/**
		Converts inner temporary storage
		to Arguments
	*/
	const Arguments collect() {
		if (temporary.begin() == temporary.end())
			return {};

		std::forward_list<std::shared_ptr<Value>> merged;
		size_t count = 0;

		std::shared_ptr<Value> current;
		bool isCurrentPresent = false;

		auto it = temporary.begin();
		it->isSeam = false;

		while (it != temporary.end()) {
			if (it->isSeam) {
				merged.push_front(current);
				isCurrentPresent = false;
				count++;
			}

			if (!isCurrentPresent) {
				isCurrentPresent = true;
				current = it->value;
			} else {
				current = std::make_shared<StringValue>(
					it->value->toString() + current->toString()
				);
			}

			it++;
		}

		if (isCurrentPresent) {
			merged.push_front(current);
			count++;
		}

		Arguments args = Arguments(count);
		std::copy(merged.begin(), merged.end(), args.begin());
		return args;
	}
};
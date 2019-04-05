#include <iostream>
#include <unordered_map>
#include <string>

#include "state.h"
#include "classic_parser.h"
#include "classic_processor.h"
#include "platform.h"


int main(int argc, char * argv[]) {
	// speed up
	std::ios::sync_with_stdio(false);

	State global = State(std::unordered_map<std::string, std::string> {
		{"prompt", "print [$ ]"}
	});

	ClassicParser parser;
	ClassicProcessor processor;

	parser.processor = &processor;
	processor.parser = &parser;

	if (Platform::isInteractive())
		parser.interact(global, std::cin, std::cout);
	else
		parser.parseAll(global, std::cin, std::cout);

	return 0;
}
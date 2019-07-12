#pragma once

#include "parser.h"

#include <memory>

#include <iostream>
#include <stack>

#include "state.h"
#include "values/value.h"
#include "values/string_value.h"

#include "arguments.h"
#include "processor.h"


/**
	Manages input analysis
*/
class ClassicParser : public Parser {
private:
	/**
		Moves forward while the current symbol
		is blank or not the terminator
	*/
	bool hasSomething(
		ArgumentsCollector & collector,
		std::istream & input,
		char terminator
	) {
		while (
			input.peek() == '\n' ||
			input.peek() == '\r' ||
			input.peek() == '\t' ||
			input.peek() == ' '
		) {
			if (input.peek() == terminator)
				return false;
			collector.markSeam();
			input.get();
		}

		return input.peek() != EOF && input.peek() != terminator;
	}

	/**
		Reads text & performs state changes
	*/
	void readSymbol(
		State & state,
		std::istream & input,
		std::ostream & token,
		std::ostream & output
	) {
		char next = input.get();

		if (next == '\\')
			token << (char) input.get();
		else if (next == '{')
			readRepresentation(input, token);
		else if (next == '[')
			readGrouping(state, input, token, output);
		else if (next == '(')
			token << readExecution(state, input, output, ')')->toString();
		else
			token << next;
	}

	/**
		Keeps spaces
	*/
	void readGrouping(
		State & state,
		std::istream & input,
		std::ostream & token,
		std::ostream & output
	) {
		while (
			input.peek() != EOF &&
			input.peek() != ']'
		) readSymbol(state, input, token, output);
		input.get();
	}

	/**
		Keeps it's content as it is but
		checks for paired `{}` recursively
	*/
	void readRepresentation(std::istream & input, std::ostream & token) {
		int depth = 1;

		while (
			input.peek() != EOF &&
			!(depth == 1 && input.peek() == '}')
		) {
			char next = input.get();

			if (next == '\\') {
				token << next;
				next = input.get();
			}

			else if (next == '{')
				depth++;
			else if (next == '}')
				depth--;

			token << (char) next;
		}

		input.get();
	}

	/**
		Token is something that is wrapped
		with blanks around.
	*/
	std::shared_ptr<Value> readToken(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		std::stringstream token;

		while (
			input.peek() != terminator &&
			input.peek() != EOF  &&
			input.peek() != '\t' &&
			input.peek() != '\n' &&
			input.peek() != '\r' &&
			input.peek() != ' '
		) readSymbol(state, input, token, output);

		return std::make_shared<StringValue>(token.str());
	}

	/**
		Splits input into the list of tokens.
	*/
	void readArguments(
		State & state,
		ArgumentsCollector & collector,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		while (hasSomething(collector, input, terminator)) {
			if (input.peek() == '(') {
				input.get();
				collector.add(readExecution(state, input, output, ')'));
			} else {
				collector.add(readToken(state, input, output, terminator));
			}
		}

		input.get();
	}

	/**
		Reads a single command until EOF or
		the terminator is met and returns the
		result of it's evaluation
	*/
	std::shared_ptr<Value> readExecution(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		ArgumentsCollector collector;
		readArguments(state, collector, input, output, terminator);
		return processor->execute(state, collector.collect(), output);
	}

public:
	Processor * processor = nullptr;

	/**
		Evaluates one command
	*/
	virtual void parse(
		State & state,
		std::istream & input,
		std::ostream & output
	) override {
		output << readExecution(state, input, output, '\n')->toString();
	}

	/**
		Evaluates everything until EOF
	*/
	virtual void parseAll(
		State & state,
		std::istream & input,
		std::ostream & output
	) override {
		while (input.peek() != EOF && !state.shouldExit)
			parse(state, input, output);
	}

	/**
		Interaction mode
	*/
	virtual void interact(
		State & state,
		std::istream & input,
		std::ostream & output
	) override {
		// we can't check for `input.peek() != EOF`
		// because it'll wait until user types anything
		while (!state.shouldExit) {
			std::stringstream prompt = std::stringstream("exec prompt");
			parse(state, prompt, output);

			if (input.peek() == EOF)
				state.shouldExit = true;

			parse(state, input, output);
		}
	}
};
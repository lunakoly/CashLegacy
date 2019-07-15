#pragma once

#include "parser.h"

#include <memory>

#include <iostream>
#include <sstream>
#include <stack>

#include "state.h"
#include "values/value.h"
#include "values/string_value.h"
#include "values/code_value.h"

#include "processor.h"


/**
	Manages input analysis
*/
class ClassicParser : public Parser {
private:
	/**
		Reads things that start with \
	*/
	char readEscapeSequence(std::istream & input) {
		char next = input.get();

		if (next == 'n')
			return '\n';

		if (next == 't')
			return '\t';

		return next;
	}

	/**
		Reads a part of a StringValue
	*/
	void readSymbol(
		State & state,
		std::istream & input,
		std::ostream & token,
		std::ostream & output
	) {
		char next = input.get();

		if (next == '\\')
			token << readEscapeSequence(input);
		else if (next == '\"')
			readGrouping(state, input, token, output);
		else if (next == '\'')
			readLiteral(input, token);
		else if (next == '(')
			token << readExecution(state, input, output, ')')->toString();
		else
			token << next;
	}

	/**
		Reads a part of a StringValue preserving whitespaces
	*/
	void readGrouping(
		State & state,
		std::istream & input,
		std::ostream & token,
		std::ostream & output
	) {
		while (
			input.peek() != EOF &&
			input.peek() != '\"'
		) readSymbol(state, input, token, output);
		input.get();
	}

	/**
		Reads a part of a StringValue preserving everything
	*/
	void readLiteral(std::istream & input, std::ostream & token) {
		while (
			input.peek() != EOF &&
			input.peek() != '\''
		) {
			char next = input.get();

			if (next == '\\')
				token << readEscapeSequence(input);
			else
				token << next;
		}

		input.get();
	}

	/**
		Reads a single StringValue
	*/
	std::shared_ptr<StringValue> readString(
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
		Tries to read a substitution and save it's Value
		or returns a StringValue
	*/
	std::shared_ptr<Value> readStringOrSubstitution(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		std::shared_ptr<Value> value = std::make_shared<StringValue>("");

		if (input.peek() == '(') {
			input.get();
			value = readExecution(state, input, output, ')');

			if (
				input.peek() == terminator ||
				input.peek() == '\t' ||
				input.peek() == '\n' ||
				input.peek() == '\r' ||
				input.peek() == ' '
			) return value;
		}

		return std::make_shared<StringValue>(
			value->toString() + readString(state, input, output, terminator)->toString()
		);
	}

	/**
		Reads a single CodeValue
	*/
	std::shared_ptr<CodeValue> readCode(std::istream & input) {
		std::stringstream token;
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
		return std::make_shared<CodeValue>(token.str());
	}

	/**
		Moves forward while the current symbol
		is blank or not the terminator
	*/
	bool hasSomething(std::istream & input, char terminator) {
		while (
			input.peek() == '\n' ||
			input.peek() == '\r' ||
			input.peek() == '\t' ||
			input.peek() == ' '
		) {
			if (input.peek() == terminator)
				return false;
			input.get();
		}

		return input.peek() != EOF && input.peek() != terminator;
	}

	/**
		Splits input into the list of tokens.
	*/
	void readArguments(
		State & state,
		Arguments & arguments,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		while (hasSomething(input, terminator)) {
			if (input.peek() == '{') {
				input.get();
				arguments.push_back(readCode(input));
			} else {
				arguments.push_back(readStringOrSubstitution(state, input, output, terminator));
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
		Arguments arguments;
		readArguments(state, arguments, input, output, terminator);
		return processor->execute(state, arguments, output);
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
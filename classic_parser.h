#pragma once

#include "parser.h"

#include <iostream>
#include <stack>

#include "arguments.h"
#include "state.h"
#include "processor.h"


/**
	Manages input analysis
*/
class ClassicParser : public Parser {
private:
	/**
		Moves forward while the current symbol
		is \t, space or \n
	*/
	void skipIndent(std::istream & input) {
		while (
			input.peek() == '\n' ||
			input.peek() == '\t' ||
			input.peek() == ' '
		) input.get();
	}

	/**
		Reads text & performs state changes
	*/
	void readSymbol(State & state, std::istream & input, std::ostream & output) {
		char next = input.get();

		if (next == '\\')
			output << (char) input.get();
		else if (next == '<')
			readExecution(state, input, output);
		else if (next == '{')
			readRepresentation(input, output);
		else if (next == '(')
			readEvaluation(state, input, output);
		else if (next == '[')
			readGrouping(state, input, output);
		else
			output << next;
	}

	/**
		Reads text & wrapps substitutions with `{}`
	*/
	void readUnescapedSymbol(State & state, std::istream & input, std::ostream & output) {
		char next = input.get();

		if (next == '\\')
			output << '\\' << (char) input.get();

		else if (next == '<') {
			output << '{';
			readExecution(state, input, output);
			output << '}';
		}

		else if (next == '(') {
			output << '{';
			readEvaluation(state, input, output);
			output << '}';
		}

		else if (next == '[') {
			output << '{';
			readGrouping(state, input, output);
			output << '}';
		}

		else if (next == '{') {
			output << '{';
			readRepresentation(input, output);
			output << '}';
		}

		else
			output << next;
	}


	/**
		Token is something that is wrapped
		with blanks around.
	*/
	void readToken(
		State & state,
		std::istream & input,
		std::ostream & output
	) {
		while (
			input.peek() != EOF &&
			input.peek() != '\t' &&
			input.peek() != '\n' &&
			input.peek() != ' '
		) readSymbol(state, input, output);
	}

	/**
		Keeps spaces inside and acts as a
		solid parameter inside `<>`
	*/
	void readGrouping(State & state, std::istream & input, std::ostream & output) {
		while (
			input.peek() != EOF &&
			input.peek() != ']'
		) readSymbol(state, input, output);
		input.get();
	}

	/**
		Keeps it's content as it is but
		checks for paired `{}` recursively
	*/
	void readRepresentation(std::istream & input, std::ostream & output) {
		int depth = 1;

		while (
			input.peek() != EOF &&
			!(depth == 1 && input.peek() == '}')
		) {
			char next = input.get();

			if (next == '\\')
				next = input.get();
			else if (next == '{')
				depth++;
			else if (next == '}')
				depth--;

			output << (char) next;
		}

		input.get();
	}

	/**
		Substitutes variable value
	*/
	void readEvaluation(State & state, std::istream & input, std::ostream & output) {
		std::stringstream sub;

		while (
			input.peek() != EOF &&
			input.peek() != ')'
		) {
			char next = input.get();

			if (next == '\\')
				next = input.get();

			sub << (char) next;
		}

		input.get();
		std::string name = sub.str();

		if (name.size() > 0)
			output << state[name];
	}

	/**
		Splits input into the list of tokens.
	*/
	void readArguments(
		State & state,
		ArgumentsCollector & collector,
		std::istream & input
	) {
		while (input.peek() != EOF) {
			skipIndent(input);

			// trim trailing spaces
			if (input.peek() == EOF)
				break;

			std::stringstream token;
			readToken(state, input, token);
			collector.add(token.str());
		}

		input.get();
	}

	/**
		Performs preprocessing. Reads a part of input
		that will be passed to arguments parsing step
	*/
	void readSubstitution(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		while (
			input.peek() != EOF &&
			input.peek() != terminator
		) {
			if (input.peek() == '$') {
				input.get();
				readSymbol(state, input, output);
			} else {
				readUnescapedSymbol(state, input, output);
			}
		}

		input.get();
	}

	/**
		Substutes the result of another command
	*/
	void readExecution(State & state, std::istream & input, std::ostream & output) {
		std::stringstream command;
		readSubstitution(state, input, command, '>');

		ArgumentsCollector collector;
		readArguments(state, collector, command);

		processor->execute(state, collector.collect(), output);
	}


public:
	Processor * processor = nullptr;

	/**
		Evaluates one command
	*/
	virtual void parse(State & state, std::istream & input, std::ostream & output) {
		std::stringstream command;
		readSubstitution(state, input, command, '\n');

		ArgumentsCollector collector;
		readArguments(state, collector, command);

		processor->execute(state, collector.collect(), output);
	}

	/**
		Evaluates everything until EOF
	*/
	virtual void parseAll(State & state, std::istream & input, std::ostream & output) {
		while (input.peek() != EOF && !state.shouldExit)
			parse(state, input, output);
	}

	/**
		Interaction mode
	*/
	virtual void interact(State & state, std::istream & input, std::ostream & output) {
		// we can't check for `input.peek() != EOF`
		// because it'll wait until user types anything
		while (!state.shouldExit) {
			std::stringstream prompt = std::stringstream("prompt");
			parseAll(state, prompt, output);

			if (input.peek() == EOF)
				state.shouldExit = true;

			parse(state, input, output);
		}
	}
};
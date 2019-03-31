#pragma once

#include "parser.h"

#include <iostream>

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
		is \t or space
	*/
	void skipIndent(std::istream & input) {
		while (
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
		Token is something that is wrapped
		with blanks around. Terminator char
		is used to determine the end of
		`<>` block
	*/
	void readToken(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		while (
			input.peek() != EOF &&
			input.peek() != '\t' &&
			input.peek() != '\n' &&
			input.peek() != ' ' &&
			input.peek() != terminator
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
		Terminator char is needed to stop inside
		`<>` substitution
	*/
	const Arguments & readArguments(
		State & state,
		std::istream & input,
		std::ostream & output,
		char terminator
	) {
		ArgumentsCollector collector;

		while (
			input.peek() != EOF &&
			input.peek() != terminator
		) {
			skipIndent(input);

			// trim trailing spaces if
			// terminator is '\n'
			if (
				input.peek() == EOF ||
				input.peek() == terminator
			) break;

			std::stringstream sub;
			readToken(state, input, sub, terminator);

			collector.add(sub.str());
		}

		input.get();
		return collector.collect();
	}

	/**
		Substutes the result of another command
	*/
	void readExecution(State & state, std::istream & input, std::ostream & output) {
		const Arguments & args = readArguments(state, input, output, '>');
		processor->execute(state, args, output);
	}


public:
	Processor * processor = nullptr;

	/**
		Evaluates one command
	*/
	virtual void parse(State & state, std::istream & input, std::ostream & output) {
		const Arguments & args = readArguments(state, input, output, '\n');
		processor->execute(state, args, output);
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
			std::stringstream prompt = std::stringstream(state["prompt"]);
			parseAll(state, prompt, output);

			if (input.peek() == EOF)
				state.shouldExit = true;

			parse(state, input, output);
		}
	}
};
#pragma once


/**
	Provides basic parsing functionality
*/
struct Parser {
	~Parser() {}

	/**
		Evaluates one command
	*/
	virtual void parse(State &, std::istream &, std::ostream &) = 0;

	/**
		Evaluates everything until EOF
	*/
	virtual void parseAll(State &, std::istream &, std::ostream &) = 0;

	/**
		Interaction mode
	*/
	virtual void interact(State &, std::istream &, std::ostream &) = 0;
};
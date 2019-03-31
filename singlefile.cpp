#include <iostream>
#include <sstream>

#include <unordered_map>
#include <forward_list>
#include <vector>


// these types are too long to type.
// merely aliases
using ArgumentsList = std::forward_list<std::stringstream *>;
using Arguments 	= std::vector<std::stringstream *>;

/**
	Converts forward_list to vector.
	Also reverses the order
*/
Arguments & toVector(ArgumentsList * list, int count) {
	Arguments * args = new Arguments(count);
	auto it = list->begin();
	int that = count - 1;

	while (
		it != list->end() &&
		that >= 0
	) {
		(*args)[that] = *it;
		that--;
		it++;
	}

	return *args;
}


/**
	Evaluates text
*/
void evaluate(std::istream &, std::ostream &);


class Scope;
extern Scope * currentScope;

/**
	Stores values of variables
	and supports hierarchy
*/
class Scope {
private:
	std::unordered_map<std::string, std::string> variables;
	Scope * parent;

public:
	Scope(
		std::unordered_map<std::string, std::string> variables = {},
		Scope * parent = nullptr
	) : parent(parent), variables(variables) {}

	std::string & operator [] (const std::string & key) {
		auto it = variables.find(key);

		if (it == variables.end() && parent != nullptr)
			return (*parent)[key];

		return variables[key];
	}

	void erase(const std::string & key) {
		variables.erase(key);
	}

	/**
		Returns true if the requested function
		has been found and executed.
	*/
	bool tryCall(const std::string & command, const Arguments & args, std::ostream & output) const {
		auto it = variables.find(command);

		if (it != variables.end()) {
			std::stringstream content = std::stringstream(it->second);

			Scope * oldScope = currentScope;
			Scope scope({}, currentScope);
			currentScope = &scope;

			for (auto it = 1; it < args.size(); it++)
				scope["$" + std::to_string(it)] = args[it]->str();

			while (content.peek() != EOF)
				evaluate(content, output);

			currentScope = oldScope;
			return true;
		}

		if (parent != nullptr)
			return parent->tryCall(command, args, output);

		return false;
	}
};

/**
	Global variables
*/
Scope globals = Scope({
	{"exit", "0"},
	{"prompt", "print [$ ]"}
});

/**
	Used during evaluation
*/
Scope * currentScope = &globals;


/**
	Builtin command for assigning
	values to variables
*/
void define(const Arguments & args, std::ostream & output) {
	if (args.size() >= 3) {
		(*currentScope)[args[1]->str()] = args[2]->str();
	} else {
		currentScope->erase(args[1]->str());
	}
}

/**
	Builtin command for writing data
	to stdout
*/
void print(const Arguments & args, std::ostream & output) {
	if (args.size() > 1) {
		output << args[1]->str();

		for (size_t it = 2; it < args.size(); it++)
			output << ' ' << args[it]->str();
	}
}

/**
	Builtin command for writing data
	to stdout and appends newline character
*/
void echo(const Arguments & args, std::ostream & output) {
	print(args, output);
	output << '\n';
}

/**
	Builtin command for termination
*/
void quit(const Arguments & args, std::ostream & output) {
	(*currentScope)["exit"] = "1";
}

/**
	Stores references to builtins
*/
std::unordered_map<std::string, void (*)(const Arguments &, std::ostream &)> builtins = {
	{"def", &define},
	{"print", &print},
	{"echo", &echo},
	{"quit", &quit},
	{"exit", &quit}
};

/**
	Returns true if the requested function
	has been found and executed.
*/
bool tryExecute(const std::string & command, const Arguments & args, std::ostream & output) {
	auto it = builtins.find(command);

	if (it != builtins.end()) {
		(it->second)(args, output);
		return true;
	}

	return false;
}


/**
	Simply provides a grouping functionality
*/
void readText(std::istream &, std::ostream &);

/**
	Parses input in literal-reading mode.
	Verifies `{}` brackets pairs
*/
void readLiteral(std::istream &, std::ostream &);

/**
	Executes variable evaluation with arguments
*/
void readCall(std::istream &, std::ostream &);

/**
	Executes command with arguments
*/
void readExecution(std::istream &, std::ostream &);

/**
	Moves forward while the current symbol
	is \t, space or \n
*/
void skipIndent(std::istream & input) {
	while (
		input.peek() == '\t' ||
		input.peek() == '\n' ||
		input.peek() == ' '
	) input.get();
}

/**
	Moves forward while the current symbol
	is \t or space
*/
void skipMargin(std::istream & input) {
	while (
		input.peek() == '\t' ||
		input.peek() == ' '
	) input.get();
}

/**
	Reads text & performs state changes
*/
void readSymbol(std::istream & input, std::ostream & output) {
	char next = input.get();

	if (next == '\\')
		output << (char) input.get();
	else if (next == '<')
		readExecution(input, output);
	else if (next == '{')
		readLiteral(input, output);
	else if (next == '(')
		readCall(input, output);
	else if (next == '[')
		readText(input, output);
	else
		output << next;
}

/**
	Token is something that is wrapped
	with blanks around
*/
void readToken(std::istream & input, std::ostream & output, char terminator) {
	while (
		input.peek() != EOF &&
		input.peek() != '\t' &&
		input.peek() != '\n' &&
		input.peek() != ' ' &&
		input.peek() != terminator
	) readSymbol(input, output);
}

void readText(std::istream & input, std::ostream & output) {
	int depth = 1;

	while (
		input.peek() != EOF &&
		input.peek() != ']'
	) readSymbol(input, output);

	input.get();
}

void readLiteral(std::istream & input, std::ostream & output) {
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

void readCall(std::istream & input, std::ostream & output) {
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
		output << (*currentScope)[name];
}

/**
	Splits input into the list of tokens
*/
Arguments & parse(std::istream & input, std::ostream & output, char terminator) {
	ArgumentsList * list = new ArgumentsList();
	int count = 0;

	// skipIndent(input);

	while (
		input.peek() != EOF &&
		input.peek() != terminator
	) {
		skipMargin(input);

		// trailing spaces
		if (
			input.peek() == EOF ||
			input.peek() == terminator
		) break;

		std::stringstream * sub = new std::stringstream();
		readToken(input, *sub, terminator);

		list->push_front(sub);
		count++;
	}

	input.get();
	return toVector(list, count);
}

/**
	Calls the given command with args
*/
void execute(const Arguments & args, std::ostream & output) {
	if (args.size() > 0) {
		std::string command = args[0]->str();

		if (
			!tryExecute(command, args, output) &&
			!currentScope->tryCall(command, args, output)
		) {
			output << "Error > Command `" << command << "` not found\n";
		}
	}
}

void readExecution(std::istream & input, std::ostream & output) {
	Arguments & args = parse(input, output, '>');
	execute(args, output);
}

void evaluate(std::istream & input, std::ostream & output) {
	Arguments & args = parse(input, output, '\n');
	execute(args, output);
}


int main(int argc, const char * argv[]) {
	// speed up
	std::ios::sync_with_stdio(false);

	while (globals["exit"] != "1") {
		// print prompt
		std::stringstream promptInput = std::stringstream(globals["prompt"]);

		// prompt may be multiline,
		// so evaluate each one
		while (promptInput.peek() != EOF)
			evaluate(promptInput, std::cout);

		// if we do this in another place
		// user will experience strange
		// prompt to write something.
		// we must check that in case of
		// reading from a redirected stdin (file)
		if (std::cin.peek() == EOF)
			globals["exit"] = "1";

		// execute input
		evaluate(std::cin, std::cout);
	}

	return 0;
}
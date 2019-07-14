# CASH
Cash is another command line interpreter that aims to achieve both the simplicity and the efficiency. With that in mind Cash sticks to the common string-oriented command line syntax but also introduces additional enhancements for optimization purposes.

## Values and String Value
Cash command is a list of values that don't have to be the strings internally. These values are separated by whitespaces (see Separators) and when presented as raw text (without using any Cash operators) treated as _string_ values.

For example,
```
echo Hello, world!
```
is treated as three string values: `echo`, `Hello,` and `world!` - and the first one is the name of the command to execute.

It's worth noticing that Cash commands are also able to return different values and this is how the majority of type conversions is done.

## `typeof`
To find out the type of a value you can use `typeof` command. It returns the string value describing the type of it's first argument. Consider the following example:
```
typeof Hello
```
prints
```
string
```

## Operator `()`
Cash allows to substitute the result of one command as an argument for the another via `()` operator.

For example,
```
echo Working directory: (pwd)
```
may result in printing `Working directory: /` in case your working directory is `/`.

It's also possible to inject the result of a command _into_ an argument. Typing `()` alongside with anything considered to be a string value will result in cluing the two parts together into a single string value. During this the value returned from `()` is asked for it's string representation.

For example, the result of
```
typeof abc(echo Hello)
```
will always be `string` no matter what was the result of the command inside `()`.

Note, that `()` can be nested.

## Separators
By default command arguments are separated by spaces, `\t` and `\r` and the end of a single command is determined by `\n` but during command substitutions the command ends where the `()` ends and it's arguments is separated by spaces, 	`\t`, `\r` and `\n`.

## Operator `''`
Sometimes it's needed to pass a bunch of words separated by whitespaces (e. g. arbitrary text)  to the command as a single argument. This can be done via `''` operator that preserves any whitespace and _disables_ any other operators.

```
echo '(2 + 2) * 2 = 8'
```
prints
```
(2 + 2) * 2 = 8
```

## Operator `""`
In some cases it's handy to enable `()` operator in an arbitrary text but it's not possible with `''`. This is where it's reasonable to use `""` operator.

Then the substitution is performed inside of `""` the value returned from `()` is asked for it's string representation.

Typically, this operator is used for formatting output. For example,
```
echo "(username)   -   (hostname)   -   (pwd)"
```
will print the current user name, host name and working directory separated by `-` surrounded with three spaces.

## `eval`
Command substitution returns one single value but, for example, if the result is a list of words separated by a space we may want to pass them all as separate arguments to another command. To do this we can construct a new string value and then evaluate it.

Let's imagine we want to compile some sources with `g++` and we have a command `SOURCES` that'll print a list of source files separated by spaces. Then we can do it the following way:
```
eval "g++ (SOURCES) -o output"
```

## States
States are the special structures allowing multiple commands share some contextual information. Initially Cash starts with one global state.

Additional states can be created dynamically by the interpreter and form a hierarchy of nested states. Whenever the state is left it provides a return value that is an empty string value by default (if the state didn't provide another one explicitly).

To exit the current state use `exit` command. Optionally it accepts a return value as the first argument.

## Variables
Cash allows to associate a value with a specific string via defining variables.

To define a variable within the current state we can use `def` command:
```
def NAME Nick
```

Once variable is defined it creates a custom command with the same name that returns the value of a variable:
```
echo My name is (NAME)
```

Variable can be accessed from any inner state.

To change the value of an existing variable the same `def` command can be used. However if we try to change the value of a variable defined in an outer state via `def` we will define a new variable in the current state instead. To change the value of an existing variable use `set` command:
```
set NAME Adam
```

If `set` is used with an undefined variable it'll do nothing.

## Code Value and Operator `{}`
Code value is a value that stores a bunch of commands that can be executed later. To define a code value the `{}` operator is used:
```
def greet {
    echo Hello!
}
```

Unlike `()` operator `{}` can't be clued with string values but commands can still ask code value for it's string representation. This is why is's possible to echo a code value.

When a code value is executed is creates an inner state and additional values can be assigned as arguments with names like: `@1`, `@2`, etc. The overall numbers of arguments is available via variable `@COUNT`.

## `exec`
To execute a code value assigned to a variable use `exec` command. It accepts the name of the variable as the first argument and all the other values are bound as arguments starting with `@1`.

We can execute a code value from the previous example like this:
```
exec greet
```

And if we define an additional argument,
```
def greet {
    echo Hello, (@1)!
}

exec greet Nick
```
will result in
```
Hello, Nick!
```

An example with return value:
```
def launch {
    echo Preparing missiles...
    echo Launching...
    exit Done.
    echo THIS IS NOT EXECUTED
}

echo Status: (exec launch)
```
And it's result:
```
Preparing missiles...
Launching...
Status: Done.
```

This example also demonstrates that the return value is not the same as echo.

The `exec` command also makes use of lambdas possible. The following code:
```
def PI 3.14159
def E  2.71828

def forEachConstant {
	exec @1 (PI)
	exec @1 (E)
}

exec forEachConstant { echo Called for (@1) }
```
will produce:
```
Called for 3.14159
Called for 2.71828
```

It's worth mentioning that you will never need to execute code values anonymously (create a one with `{}` and pass arguments for it directly after) because that's the case to use `eval`.

If a non-code value is stored in a variable passed as first argument to `exec` then this value's string representation is executed.

## Character Escaping
To prevent Cash from treating a curtain character as an operator this character must be escaped by putting `\` before it. This will instruct interpreter to throw away `\` and treat the next character as raw text.

For example, character escaping may be useful in this case:
```
echo It\'s fun!
```

Escaping arbitrary characters with `\` will work the same way: interpreter will throw away `\` and treat them as raw text even if they already are.

## Disabling Operators
Operators may be disabled according to the context. When this happens, it means that this operator characters will be treated as raw text without the need to escape them.

For example, `()` will be disabled inside of `''` and that's why there is no need to put `\` before `()`.

## Value Optimizations
Besides of string and code values Cash provides some additional value types optimized for different tasks

### Int Value
A value designed for integer calculations that internally stores an integer.

Doesn't have strict size definitions yet.

To convert another value to an integer use `int` command:
```
def SCORE (int 100)
```

If the conversion fails or if there are no arguments provided to `int` zero int value will be returned.

The result of `typeof` is `int`.

### Float Value
A value designed for calculations that internally stores a float.

Doesn't have strict size definitions yet.

To convert another value to a floating point number use `float` command:
```
def PI (float 3.14159)
```

If the conversion fails or if there are no arguments provided to `float` zero float value will be returned.

The result of `typeof` is `float`.

## Interpreter Modes
Cash interpreter can be started in one of several modes.

### Interactive Mode
When started via terminal/console (`isatty` returns true) Cash starts in an interactive mode. Each time a user is allowed to type in a command the predefined `prompt` code value is called (`exec prompt`).

The default `prompt` definition is:
```
def prompt { print '$ ' }
```

### Non-Interactive Mode
In all cases other than starting Cash in the interaction mode it will be started in the non-interactive mode. The only difference - the prompt will not be executed.

## Built-in Commands
Cash interpreter provides a bunch of built-in commands. Some of them are closely connected to the Cash core features and others have been added to provide cross-platform way to access certain data.

### General
- `typeof` - Get the type of a value
- `exit` - Escape the current scope and optionally provide return value
- `eval` - Evaluate a string representation of a value as a command
- `def` - Define variables
- `set` - Re-assign a value to a variable
- `exec` - Execute a code value assigned to a variable

### Display
- `print` - Print string representations of all the following values with one space between them
- `echo` - Print and append `\n`

### Shorthands
- `pwd` - Print working directory
- `username` - Print user name
- `hostname` - Print host name

### Numeric
- `int` - Construct an integer from the first argument
- `float` - Construct a floating point number from the first argument


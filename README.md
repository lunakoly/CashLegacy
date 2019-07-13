# Cash
Cash is a command line language with straightforward syntax.

## Values
Cash command is a list of abstract values separated by whitespaces.
These values may or may not represent a string internally.
This allows Cash to optimize values for
different tasks.

Text enclosed in `{}` is treated as a single `code` value and is optimized for
being executed as a function body.
Text enclosed in `()` is treated as a substitution of another command and
the result of another command may be any value. If there're any non-whitespace
characters around `()` then the result of `()` will be treaded as `string` value and
concatinated with the surrounding parts.
Text that does not fit into the two upper rules is treated as `string` value.

Cash can define additional value types (`integer`, `float`) but
they should only be available via the corresponding commands (`int`, `float`).

To get the type of a value the `typeof` command can be used:
```
echo Type is (typeof (int 100))
-------------------------------
Type is integer
```

## Syntax
The general command execution syntax is:
```
command arg1 arg2 ...
```
To define a text literal single quotes `''` can be used.
They preserve the text inside as it is:
```
echo 'My name is:        Nick'
------------------------------
My name is:        Nick
```
To substitute a command there are parentheses `()`:
```
echo Current working directory: (pwd)
--------------------------------------------
Current working directory: /home/luna_koly
```
Single quotes `''` disable command substitution that may not
be very handy sometimes. For this case there is a grouping syntax `""`.
Double quotes preserve whitespaces but allow substitution:
```
echo "Current working directory:        (pwd)"
---------------------------------------------------
Current working directory:        /home/luna_koly
```
To stop Cash the command `exit` can be used.

## Variables
Cash supports binding values with some string names as variables.
Creating a variable or reassigning a value to an existing one can be done like this:
```
def NAME Nick
```
When variable is defined it creates a command that returns the value:
```
echo User Name: (NAME)
```
Variable contents can be executed as functions via `eval` or `exec` commands.
When this is done other command parameters are bound as arguments starting from 1: `@1`, `@2`, etc.
Agruments are bound to a temporary created inner scope.
`eval` commands executes the value of it's first parameter so to run a function parentheses
should be used to get it's value first. For the sake of readability there is the `exec` command that
simply accepts the name of the variable:
```
def greet {
    echo Hello, (@1)!
}

eval (greet) (pun)
exec  greet  (pun)
-----------------
Hello, luna_koly!
Hello, luna_koly!
```
Since functions define their inner scope it's unclear where exactly the `def` command should define a new variable.
Using `def` inside a function creates a local variable and thus hides the name of the outer variable
if trying to reassign it. That's why there is a `set` command that will only reassign an existing variable value
if variable exists and does nothing otherwise.
To return a value from a function use the same `exit` command:
```
def getSomeFloat {
    echo Preparing missiles...
    echo Launching...
    exit (float 3.1415)
    echo THIS IS NOT EXECUTED
}

echo Return type = (typeof (exec getSomeFloat))
-----------------------------------------------
Preparing missiles...
Launching...
Return type = float
```
Sometimes it's useful to store a number of command pre-known arguments inside one variable
and substitute it's value into a command. This is one of the cases where `eval` command comes handy:
```
def SOURCES 'a.cpp b.cpp c.cpp'
eval "g++ (SOURCES) -o output"
```
Having different ways to get variable value and execute it allows to easily implement lambdas:
```
def PI 3.14159
def E  2.71828

def forEachConstant {
	exec @1 (PI)
	exec @1 (E)
}

exec forEachConstant { echo Called for (@1) }
---------------------------------------------
Called for 3.14159
Called for 2.71828
```

## Builtin variables
- `prompt` - Function that is called before user is allowed
to type the command. You can edit it like:
```
def prompt {
    echo (pun) in (pwd)
    print '=> '
}
```

## Builtin commands
### General
- `exit` - Escapes the current scope and optionally provide some value other than the default empty string

### Values
- `typeof` - Get the type of a value
- `def` - Define variables
- `set` - Reassign a value
- `eval` - Evaluate a value as user input
- `exec` - Execute a variable content as user input

### Display
- `print` - Print text without appending newline character
- `echo` - Print and append '\n'

### Shorthands
- `pwd` - Print working directory
- `pun` - Print user name
- `phn` - Print host name

### Numeric
- `int` - Construct an integer from the first argument
- `float` - Construct a float from the first argument

## Recomended order to get accuainted with the code
1. values
1. state.h
1. platforms
1. builtins
1. processor.h
1. parser.h
1. cash.cpp
1. classic_processor.h
1. classic_parser.h

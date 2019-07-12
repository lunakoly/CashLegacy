# Cash
Straightforward command line interpreter with simple syntax

## Syntax
The general command execution syntax is:
```
command arg1 arg2 ...
```
Command name and arguments are splited by whitespaces. 
To define a text literal curly braces `{}` can be used.
They preserve the text inside of them as it is:
```
echo {My name is:        Nick}
------------------------------
My name is:        Nick
```
There is a command substitution syntax via parenthesis `()`:
```
echo Current working directory: "(pwd)"
--------------------------------------------
Current working directory: "/home/luna_koly"
```
_Note that quotes do not affect parsing._  
Text literation syntax `{}` also disables command substituion that may not
be very handy sometimes. For this case there is a grouping syntax `[]`.
Square brackets preserve whitespaces but also parse inner contents:
```
echo [Current working directory:        "(pwd)"]
---------------------------------------------------
Current working directory:        "/home/luna_koly"
```
To stop Cash the command `exit` can be used and to evaluate a value as a command there is an `eval` command:
```
eval { echo Hello! }
```

## Values
When a command is parsed it isn't split into a list of strings but
into a list of some abstract values. Probably by default they are
string values but they don't have to. This allows Cash to make
some values optimized for different tasks. These 'custom' values can be
received from some of the builtin commands like `int`:
it returns an integer value that is more suitable for calculations.
To get the type of a value we can use `typeof` command:
```
echo Type is (typeof (int 100))
-------------------------------
Type is integer
```
Builtin commands can accept and return values of different types.
Right now the available type optimizations are:
- `string`
- `integer` (returned by `int` command)
- `float` (returned by `float` command)

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
exec greet (pun)
-----------------
Hello, luna_koly!
Hello, luna_koly!
```
Since functions define their inner scope it's unclear where exactly the `def` command should define a new variable.
Using `def` inside a function creates a local variable and thus hides the name of the outer variable
if trying to reassign it. That's why there is a `set` command that will only reassign an existing variable value
if variable exists and does nothing otherwise.
To return a value from a function use the sane `exit` command:
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

## Builtin variables
- `prompt` - Function that is called before user is allowed
to type the command. You can edit it like:
```
def prompt {
    echo (pun) in (pwd)
    print {=> }
}
```

## Builtin commands
### General
- `exit` - Escapes the current scope and optionally provide some value other than the default empty string
- `eval` - Evaluate a value as user input

### Values
- `typeof` - Get the type of a value
- `def` - Define variables
- `set` - Reassign a value
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

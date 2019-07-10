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

## Values
When a command is parsed it isn't split into a list of strings but
into a list of some abstract values. Probably by default they are
string values but they don't have to. This allows Cash to make
some values optimized for some tasks. These 'custom' values can be
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
To only reassign a value of already defined variable there is one more command:
```
set NAME Adam
```
When variable is defined it creates a command that returns the value:
```
echo User Name: (NAME)
```
Variable contents can be executed as functions (providing an inner scope of variable names) via `exec`.
When this is done other arguments are bound as arguments starting from 1: `@1`, `@2`, ...
```
def greet {
    echo Hello, (@1)!
}

exec greet (pun)
-----------------
Hello, luna_koly!
```
Since functions define their inner scope it now makes sence why there're both `def` and `set` commands.
Using `def` inside a function allows to create a local variable and thus hides the name of the outer variable
if trying to reassign it.
To return a value from a function use `exit` command:
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

### Values
- `typeof` - Get the type of a value
- `def` - Define variables
- `set` - Reassign a value

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

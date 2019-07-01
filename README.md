# Cash
Straightforward command line interpreter with simple syntax

## Syntax
The main execution syntax is:
```
command arg1 arg2 ...
```
_System shell commands execution has not been implemented yet as well as execution of cash script files_

Variables can be declared via `def` command:
```
def SCORE 100
```
To get variable value _parentheses operator_ `()` can be used:
```
echo User score: (SCORE)
------------------------
User score: 100
```
Cash uses brackets instead of quotes. To save contents **without** parsing it use _curly braces operator_ `{}`:
```
def TEXT {Lorem ipsum (dolor sit amet), consectetur}
```
This way `TEXT` will store `Lorem ipsum (dolor sit amet), consectetur`.

If you need to preserve spaces but parse inner operator constructions use _square brackets operator_ `[]`:
```
echo [User score    :    (SCORE)]
---------------------------------
User score    :    100
```
To substitute another command result _subcommand operator_ `<>` can be used:
```
echo Current working directory: "<pwd>"
--------------------------------------------
Current working directory: "/home/luna_koly"
```
If you need function-like behaviour,
you can also execute variable value via command execution syntax. Function parameters are substituted as
variables with numeric names starting at 1:
```
def greet {
    echo Hello, (1)!
}
greet <pun>
--------------------
Hello, luna_koly!
```
Since parameters are also variables they can be executed too:
```
def PI 3.14159
def E  2.71828

def forEachConstant {
	1 (PI)
	1 (E)
}

forEachConstant { echo Called for (1) }
---------------------------------------
Called for 3.14159
Called for 2.71828
```
Arguments are substituted as solid blocks of text
but if you wish, you may use _alias_ operator `$` to substitute
something.  
_The substituted content will be executed as part of the command!_  
Assuming:
```
def SOURCES {main.cpp other.cpp}
```
The two following lines are equivalent:
```
g++ (SOURCES) -o out
g++ {main.cpp other.cpp} -o out
```
As well as these ones:
```
g++ $(SOURCES) -o out
g++ main.cpp other.cpp -o out
```
Operator `$` is applied to the next symbol. It literaly
disables grouping for the next substitution.
```
def test {
	echo A
	echo B
}
echo The result of `test` in one line: --$<test>--
--------------------------------------------------
The result of `test` in one line: --A B --
```

## Builtin variables
- `prompt` - Function that is called before user is allowed
to type the command. You can edit it like:
```
def prompt {
    echo <pun> in <pwd>
    print {=> }
}
```

## Builtin commands
- `def` - Define variables
- `print` - Print text without appending newline character
- `echo` - Print and append '\n'
- `exit` - You got it
- `pwd` - Print working directory
- `pun` - Print user name
- `phn` - Print host name

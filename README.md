# Cash
Straightforward command line interpreter with simple syntax

## Syntax
Execute a command with arguments:
```
command arg1 arg2 ...
```
Declare variables as:
```
def SCORE 100
```
And use them with `()`:
```
echo User score: (SCORE)
```
Cash uses brackets instead of quotes because it's simplier
to declare nested things that way.
To save contents **without** parsing it use `{}`:
```
def TEXT {Lorem ipsum (dolor sit amet), consectetur}
```
To group text and parse inner substitutions user `[]`:
```
echo [User name    :    (USER)]
```
To substitute another command result `<>` can be used:
```
echo Result: <getInfo item>
```
If you need function-like behaviour,
you can also execute variable value like this:
```
def greet {
    echo Hello, ($1)!
}
greet (USER)
```
By default variables are substituted as solid blocks of text
but if you wish, you may split the contents of a token into
multiple arguments with `$`.
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

## Builtin variables
- `prompt` - Function that is called before user is allowed
to type the command. You can edit it like:
```
def prompt {
    echo (user) (cwd)
    print [=> ]
}
```

## Builtin commands
- `def` - Define variables
- `print` - Print text without appending newline character
- `echo` - print and append '\n'
- `exit` - you got it

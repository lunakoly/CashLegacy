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
echo [User score    :    (SCORE)]
```
To substitute another command result `<>` can be used:
```
echo Current working directory: "<pwd>"
```
If you need function-like behaviour,
you can also execute variable value like this:
```
def greet {
    echo Hello, ($1)!
}
greet <pun>
```
Arguments are substituted as solid blocks of text
but if you wish, you may use _alias_ operator `$` to substitute
something before the arguments splitting step.
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
echo The result of `test` in one line: --$<test>--
```

## Builtin variables
- `prompt` - Function that is called before user is allowed
to type the command. You can edit it like:
```
def prompt {
    echo <pun> in <pwd>
    print [=> ]
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

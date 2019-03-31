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

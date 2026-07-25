# MiniShell

MiniShell is a Unix shell implementation written in C. It recreates the core behavior of Bash: command parsing, process execution, pipelines, redirections, environment management, signal handling, and built-in commands.

The goal of the project is to understand how Unix shells work internally — process creation, file descriptors, IPC, parsing into an AST, signal handling, and memory management.

---

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [Compatibility](#compatibility)
- [Contributing](#contributing)

---

## Features

### Interactive Shell

- Interactive prompt, reads and executes commands continuously.
- Command history and line editing via GNU Readline (arrow keys, cursor movement, deleting characters).

### Built-in Commands

| Command  | Description                            |
|----------|----------------------------------------|
| `echo`   | Display text, supports `-n`            |
| `cd`     | Change the current working directory   |
| `pwd`    | Print the current working directory    |
| `export` | Create or update environment variables |
| `unset`  | Remove environment variables           |
| `env`    | Display environment variables          |
| `exit`   | Exit the shell with a status code      |

### Command Execution

- Execution of external programs via absolute, relative, or `PATH`-resolved paths.
- Proper exit status handling and propagation.

### Quotes

Bash-style quoting rules:

- Single quotes (`'`) prevent all expansion.
- Double quotes (`"`) allow variable expansion.

```bash
minishell$ echo '$HOME'
$HOME

minishell$ echo "$HOME"
/home/user
```

### Environment Variables

```bash
echo $USER      # variable expansion
echo $?         # exit status expansion
```

### Redirections

| Operator | Description        |
|----------|--------------------|
| `<`      | Input redirection  |
| `>`      | Output redirection |
| `>>`     | Append output      |
| `<<`     | Here-document      |

```bash
cat < input.txt
echo hello > output.txt
cat << EOF
Hello
EOF
```

### Pipes

```bash
ls -la | grep minishell
```

Pipelines connect processes by wiring their file descriptors together.

### Signal Handling

| Signal  | Behavior                   |
|---------|----------------------------|
| `Ctrl-C` | Interrupt current command |
| `Ctrl-D` | Exit shell                |
| `Ctrl-\` | Quit running process      |

### Bonus Features

- **Wildcard expansion (`*`)** — expanded before command execution.

	```bash
	minishell$ ls *.c
	main.c utils.c parser.c
	```

- **Logical operators (`&&`, `||`)** — respect operator precedence.

	```bash
	mkdir test && cd test
	false || echo "Command failed"
	```

- **Parentheses / subshells** — grouped commands run in a separate subshell environment.

	```bash
	(echo hello && echo world)
	```

---

## Architecture

MiniShell processes input through a multi-stage pipeline:

```
User Input
    │
    ▼
Scanner   → tokenizes words, quotes, operators, pipes, redirections, parentheses
    │
    ▼
Parser    → builds an Abstract Syntax Tree (AST), applies operator precedence
    │
    ▼
Executor  → walks the AST: builtins, external commands, pipelines, redirections
```

**Scanner** — tokenizes raw input and validates it before parsing.

**Parser** — converts tokens into an AST that captures operator precedence, pipes, logical operators, subshells, and redirections.

Example — `echo hello && (ls | grep txt)`:

```
        &&
       /  \
   echo   ()
           │
         PIPE
         /  \
       ls   grep
```

**Executor** — traverses the AST and runs commands: builtin execution, forking processes, `execve()`, pipe wiring, redirections, subshell execution, and exit status propagation.

---

## Installation

### Requirements

- C compiler (`cc` or `clang`)
- GNU Make
- GNU Readline

### Linux

```bash
sudo apt-get update -y
sudo apt-get install build-essential -y
sudo apt-get install libreadline-dev -y

git clone https://github.com/jesuismarie/minishell.git
cd minishell
make
./minishell
```

### macOS

```bash
git clone https://github.com/jesuismarie/minishell.git
cd minishell
make configure
make
./minishell
```

### Makefile Commands

| Command      | Description                        |
|--------------|------------------------------------|
| `make`       | Compile MiniShell                  |
| `make clean` | Remove object files                |
| `make fclean`| Remove object files and executable |
| `make re`    | Recompile the project              |

---

## Usage

```bash
$ echo Hello, MiniShell!
Hello, MiniShell!

$ echo -n Hello
Hello

$ pwd
/home/user/minishell

$ cd /tmp

$ export MY_VARIABLE=value
$ echo $MY_VARIABLE
value

$ ls -l | grep ".txt"

$ cat input.txt > output.txt
$ echo World >> output.txt
$ cat < output.txt

$ cat << EOF
> Hello
> MiniShell
> EOF
Hello
MiniShell

$ env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOME=/Users/user

$ exit
```

### Logical Operators

```bash
$ mkdir project && cd project

$ cd missing_directory || echo "Directory not found"
Directory not found
```

### Subshells

```bash
$ (echo hello && echo world)
hello
world

$ (cd /tmp && pwd)
```

The original shell's directory remains unchanged after the subshell exits.

### Exit Status

```bash
$ ls existing_file
$ echo $?
0

$ ls missing_file
ls: cannot access 'missing_file': No such file or directory
$ echo $?
2
```

---

## Testing

Additional test files are provided:

```
cases.txt
```

They cover command combinations, edge cases, parser tests, error handling, redirections, and logical operators.

```bash
./minishell < cases.txt
```

### Bash Compatibility

Compare behavior directly against Bash:

```bash
echo hello
echo "$HOME"
echo '$HOME'
export TEST=value
unset TEST
ls | grep c
cat << EOF
hello
EOF
```

---

## Project Structure

```text
.
├── includes/
├── sources/
│   ├── built-in/
│   ├── scanner/
│   ├── parse/
│   ├── execute/
│   ├── signal/
│   ├── history/
│   ├── utils/
│   └── minishell.c
├── Libft/
├── readline-8.2/
├── cases.txt
├── Makefile
├── LICENSE
├── NOTICE
└── README.md
```

---

## Dependencies

- C compiler: GCC or Clang
- GNU Make
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) — command history, cursor movement, line editing

---

## Compatibility

Tested on Linux and macOS, x86_64 and ARM64 (Apple Silicon).

---

## Contributing

Contributions are welcome. Before submitting changes:

1. Follow the existing coding style.
2. Make sure the project compiles without warnings: `make`
3. Test your changes: `./minishell`

See [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines.

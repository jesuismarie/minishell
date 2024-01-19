# MiniShell 

MiniShell is a simplified shell program developed as a project for the 42 school. It provides a basic command-line interface where users can execute various commands and manage processes within a Unix-like environment. This project aims to deepen your understanding of processes, file descriptors, and signal handling.

## Features

MiniShell provides the following features:

- Display a user-friendly prompt when waiting for a new command, providing an intuitive user experience.
- Command execution: Users can execute built-in commands (e.g., `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`) as well as external commands available in the system.
- Command line editing: Users can navigate and edit command lines using the arrow keys and basic line editing functionalities (e.g., moving the cursor, deleting characters).
- Support single quotes `'` , preventing the shell from interpreting characters within the quoted sequence, allowing users to work with literal strings.
- Support double quotes `"` , preventing the shell from interpreting characters within the quoted sequence, except for the `$` (dollar sign) for variable expansion.
- Redirections: MiniShell supports input and output redirections using the `<`, `>`, `>>`, `<<` operators.
- Pipes: Users can pipe the output of one command as input to another command using the `|` operator.
- Environment variables: Users can manage environment variables by setting, unsetting, and displaying their values.
- Signals: MiniShell handles signals such as `Ctrl-C` and `Ctrl-\` to interrupt or terminate the running commands. It also handles the `Ctrl-D` signal to gracefully exit the shell.
- Error handling: MiniShell provides appropriate error messages and handles errors gracefully.
- Support special variables, such as `$?`, for context-based command behavior.
- History: MiniShell maintains a command history, allowing users to navigate and re-execute previous commands using the arrow keys.
- Echo option `-n`: The `echo` command supports the `-n` option to suppress the trailing newline character.
- Wildcard `*`: MiniShell supports the use of the wildcard `*` to match any sequence of characters in file or directory names.
- Logical operators `&&` and `||` with parentheses: MiniShell supports logical operators for command chaining, including `&&` (AND) and `||` (OR), and users can use parentheses to control the order of execution and define priority.

With these features, MiniShell offers a powerful and versatile command-line experience, enabling users to efficiently navigate the shell environment, execute commands with various options and conditions, and handle signals for interruption and termination. The support for wildcards, logical operators, and command grouping further enhances the flexibility and functionality of Minishell.

## Compatibility

MiniShell is compatible with:

- MacOS
- Linux

## Getting Started

### For Linux

1. Before running Minishell on Linux, ensure that you have the necessary dependencies installed. You may need to install the following packages:

   ```bash
   sudo apt-get update -y
   sudo apt-get install build-essential -y
   sudo apt-get install libreadline-dev -y
   ```

2. Clone the repository:

   ```bash
   git clone https://github.com/jesuismarie/minishell.git
   ```

3. Change into the project directory:

   ```bash
   cd minishell
   ```

4. Compile the program:

   ```bash
   make
   ```

5. Run MiniShell:

   ```bash
   ./minishell
   ```

6. You can now start executing commands and exploring the functionalities of Minishell.

### For MacOS

To get started with MiniShell, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/jesuismarie/minishell.git
   ```

2. Change into the project directory:

   ```bash
   cd minishell
   ```

3. Configure Readline Library

   ```bash
   make configure
   ```

4. Compile the program:

   ```bash
   make
   ```

5. Run MiniShell:

   ```bash
   ./minishell
   ```

6. You can now start executing commands and exploring the functionalities of Minishell.

## Usage

Minishell provides a command-line interface where you can type commands and execute them. Here are some examples:

- Execute a built-in command:

  ```bash
  $ echo Hello, MiniShell!
  Hello, MiniShell!
  ```

- Execute an external command:

  ```bash
  $ ls -l
  total 12
  -rw-r--r-- 1 user group   78 Jul  1 15:23 README.md
  -rwxr-xr-x 1 user group 9104 Jul  1 15:23 minishell
  ```

- Change directory:

  ```bash
  $ cd /path/to/directory
  ```

- Set an environment variable:

  ```bash
  $ export MY_VARIABLE=value
  ```

- Redirect input/output:

  ```bash
  $ cat input.txt > output.txt
  ```

- Pipe commands:

  ```bash
  $ ls -l | grep ".txt"
  ```

- Display environment variables:

  ```bash
  $ env
  PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
  HOME=/Users/user
  ```

- Exit MiniShell:

  ```bash
  $ exit
  ```

## Additional Test Cases

To further test the capabilities of Minishell, you can use the provided `cases.txt` file. This file contains additional test cases that cover various scenarios, including complex command combinations, edge cases, and error handling.

For more information on the available commands and features, please refer to the project documentation.

## Project Structure

The project structure is as follows:

- `sources/`: Contains the source code files.
- `includes/`: Contains the header files.
- `Libft/`: A library of useful functions for string manipulation, memory allocation, etc.
- `Makefile`: The Makefile for compiling the project.
- `readline/`: Readline Library.

## Dependencies

MiniShell has the following dependencies:

- C compiler (e.g., CC)
- Make

## Contributing

We'd love to have you contribute! Please refer to our [contribution guidelines](./CONTRIBUTING.md) for details.

## License

[**Apache 2.0 License**](./LICENSE)

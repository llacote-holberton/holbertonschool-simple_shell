# Simple Shell: Command Line Interface to interact with your OS.

### Summary
Simple Shell is a simple command line interface program allowing user to send simple commands to manipulate files and get information on OS.
It supports the most essential features a user could expect: support of user $PATH to find programs, propagation of command arguments,
 error handling, interactive and non-interactive mode.

## How to install and run
### Prerequisites
You must have the packages/tools related to C
  installed on your system, as well as a "compiler"
  (program used to "prepare" this tool for your machine).
In case that helps here are online resources to install
  those tools depending on your operating system
  * Windows: https://learn.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=msvc-170
     and https://nullprogram.com/blog/2016/06/13/
  * Mac: https://www.cs.auckland.ac.nz/~paul/C/Mac/
  * Linux (deb package system): https://jvns.ca/blog/2025/06/10/how-to-compile-a-c-program/

  ### Downloading
If you have git and are comfortable with command line,
  you can simply open one and go to the directory in which you want calculator to be.
  Then run (without the quotes) `"git clone https://github.com/llacote-holberton/holbertonschool-simple_shell.git"`
Otherwise you can simply download a zip containing all projects file
  by following [this url](https://github.com/llacote-holberton/holbertonschool-simple_shell/archive/refs/heads/main.zip)
  then unfolding it where you want on your computer.

  ### Starting program
Open a terminal place yourself in the directory in which you have downloaded/extracted the project then compiled an executable,  
and just type ./<executable-name>  (for example for the "demo executable" it would be `./custom_shell_demo.out`).

### Key Functions

| File | Function | Description |
|------|----------|-------------|
| `main.c` | `main()` | Main loop: prompt, read input, process commands |
| `main.c` | `get_input_line()` | Reads user input with getline |
| `main.c` | `process_input()` | Tokenizes and executes commands |
| `builtins.c` | `execute_builtin()` | Checks and executes built-in commands |
| `builtins.c` | `builtin_exit()` | Handles exit command |
| `builtins.c` | `builtin_env()` | Handles env command |
| `path.c` | `get_cmd_fullpath()` | Resolves command path using PATH |
| `tokenize.c` | `tokenize_string()` | Splits input into tokens |
| `execute.c` | `execute_command()` | Forks and executes external commands |

## Process Flow

---
config:
  theme: neo-dark
---
flowchart TB
    START(["🐚 SIMPLE SHELL - Démarrage"]) --> INIT["Initialiser variables<br>line, args, env<br>line_number = 0"]
    INIT --> LOOP{"🔄 Boucle Principale<br>while 1"}
    LOOP --> CHECK_TTY{"Mode interactif ?<br>isatty STDIN"}
    CHECK_TTY -- OUI --> PRINT_PROMPT["Afficher prompt<br>'$ '"]
    CHECK_TTY -- NON --> NO_PROMPT["Pas de prompt"]
    PRINT_PROMPT --> READ["Lire entrée utilisateur<br>getline"]
    NO_PROMPT --> READ
    READ --> CHECK_EOF{"EOF détecté ?<br>Ctrl+D"}
    CHECK_EOF -- OUI --> PRINT_NEWLINE{"Mode interactif ?"}
    PRINT_NEWLINE -- OUI --> PRINT_NL["Afficher \\n"]
    PRINT_NEWLINE -- NON --> FREE_EXIT
    PRINT_NL --> FREE_EXIT["Libérer mémoire<br>free line"]
    FREE_EXIT --> END(["👋 Fin du Shell"])
    CHECK_EOF -- NON --> CHECK_EMPTY{"Ligne vide ?<br>line[0] == '\\0'"}
    CHECK_EMPTY -- OUI --> LOOP
    CHECK_EMPTY -- NON --> INCREMENT["line_number++"]
    INCREMENT --> PARSE["Découper la ligne<br>strtok avec délimiteurs<br>espaces, tabs, newlines"]
    PARSE --> CHECK_ARGS{"tokens vide ?<br>tokens[0] == NULL"}
    CHECK_ARGS -- OUI --> FREE_LOOP["Libérer mémoire<br>free tokenized_string<br>free tokens"]
    CHECK_ARGS -- NON --> CHECK_BUILTIN{"Built-in ?<br>exit ou env"}
    CHECK_BUILTIN -- exit --> BUILTIN_EXIT["Built-in: exit<br>Libérer mémoire<br>Quitter le shell"]
    BUILTIN_EXIT --> END
    CHECK_BUILTIN -- env --> BUILTIN_ENV["Built-in: env<br>Afficher variables<br>d'environnement"]
    BUILTIN_ENV --> FREE_LOOP
    CHECK_BUILTIN -- NON --> FIND_PATH["Chercher commande<br>get_cmd_fullpath<br>Si chemin absolu/relatif → utiliser tel quel<br>Sinon → chercher dans PATH"]
    FIND_PATH --> CHECK_FOUND{"Commande<br>trouvée ?"}
    CHECK_FOUND -- NON --> ERROR_NOTFOUND["Afficher erreur stderr<br>'argv[0]: line_number: cmd: not found'<br>print_error"]
    ERROR_NOTFOUND --> FREE_LOOP
    CHECK_FOUND -- OUI --> FORK["Créer processus enfant<br>fork"]
    FORK --> CHECK_FORK{"fork<br>retourne ?"}
    CHECK_FORK -- "pid == 0" --> CHILD["👶 PROCESSUS ENFANT<br>pid == 0"]
    CHECK_FORK -- pid > 0 --> PARENT["👨 PROCESSUS PARENT<br>pid > 0"]
    CHECK_FORK -- "pid == -1" --> FORK_ERROR["Erreur fork<br>perror"]
    FORK_ERROR --> FREE_LOOP
    CHILD --> EXECVE["Remplacer par commande<br>execve pathname, args, env"]
    EXECVE --> EXECVE_ERROR["Si execve échoue<br>perror + exit"]
    PARENT --> WAIT["Attendre fin enfant<br>wait ou waitpid"]
    WAIT --> FREE_CMD["Libérer command_fullpath"]
    FREE_CMD --> FREE_LOOP
    FREE_LOOP --> LOOP

### PATH Resolution

The `get_cmd_fullpath()` function searches for executable commands in the system PATH environment variable.

**Key concepts:**
- `getenv()` - Retrieves PATH environment variable
- `strtok()` - Splits PATH into individual directories
- `stat()` or `access()` - Verifies file exists and is executable
- Returns full path if found, `NULL` otherwise

**Examples:**

```c
// Absolute path - returns immediately
get_cmd_fullpath("/bin/ls", env)  → "/bin/ls" (if exists)

// Relative path - returns immediately  
get_cmd_fullpath("./hsh", env)    → "./hsh" (if exists)

// Command name - searches PATH
get_cmd_fullpath("ls", env)       → "/bin/ls" (found in /bin)

// Invalid command
get_cmd_fullpath("invalid", env)  → NULL (not found)
```

## Testing

### Manual Testing

Use the test script to verify functionality:

```bash
# Test built-ins
echo "exit" | ./hsh
echo "env" | ./hsh

# Test external commands
echo "ls" | ./hsh
echo "ls -la" | ./hsh
echo "/bin/pwd" | ./hsh

# Test error handling
echo "nonexistent" | ./hsh
echo "" | ./hsh
```

### Memory Leak Testing

Verify no memory leaks with Valgrind:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./hsh
# Then type commands and exit
```

Expected result: **"All heap blocks were freed -- no leaks are possible"**

### Comparison with /bin/sh

```bash
# Test your shell
echo "ls -l" | ./hsh > output_hsh.txt

# Test sh
echo "ls -l" | /bin/sh > output_sh.txt

# Compare
diff output_hsh.txt output_sh.txt
```

### Automated Test Suite

Create a file `test_shell.sh`:

```bash
#!/bin/bash

echo "===== SIMPLE SHELL TESTS ====="

# Test 1: Basic commands
echo "Test 1: ls"
echo "ls" | ./hsh

# Test 2: Built-in exit
echo "Test 2: exit"
echo "exit" | ./hsh

# Test 3: Built-in env
echo "Test 3: env"
echo "env" | ./hsh | head -5

# Test 4: Command with arguments
echo "Test 4: ls -l"
echo "ls -l" | ./hsh

# Test 5: Absolute path
echo "Test 5: /bin/pwd"
echo "/bin/pwd" | ./hsh

# Test 6: Command not found
echo "Test 6: nonexistent"
echo "nonexistent" | ./hsh

# Test 7: Empty input
echo "Test 7: empty line"
echo "" | ./hsh

# Test 8: Multiple commands
echo "Test 8: multiple commands"
echo -e "ls\npwd\nexit" | ./hsh

echo "===== TESTS COMPLETE ====="
```

Run with:
```bash
chmod +x test_shell.sh
./test_shell.sh
```

## Allowed Functions and System Calls

- **String functions:** `strlen`, `strcpy`, `strcat`, `strcmp`, `strdup`, `strtok`
- **I/O:** `printf`, `fprintf`, `putchar`, `getline`, `perror`
- **Memory:** `malloc`, `free`
- **Process:** `fork`, `execve`, `wait`, `waitpid`, `exit`, `_exit`
- **File:** `access`, `stat`, `open`, `close`, `read`, `write`
- **Directory:** `opendir`, `readdir`, `closedir`
- **Environment:** `getenv`, `isatty`, `getpid`
- **Other:** `signal`, `kill`, `fflush`

## Requirements

- All code compiled with: `gcc -Wall -Werror -Wextra -pedantic -std=gnu89`
- Betty style compliant
- No memory leaks
- Maximum 5 functions per file
- All header files include guarded

## Known Limitations

- Does not handle command separators (`;`, `&&`, `||`)
- Does not handle pipes (`|`) or redirections (`>`, `<`, `>>`)
- Does not handle wildcards (`*`, `?`)
- Does not handle variables (`$VAR`)
- Does not handle quotes for arguments with spaces
- Does not handle comments (`#`)
- Does not support job control (`Ctrl+Z`, `bg`, `fg`)

## Authors

- **Soufiane Filali** - [GitHub](https://github.com/soufiane-filali)
- **Laurent Lacôte** - [GitHub](https://github.com/llacote-holberton)

## Acknowledgments

- Holberton School for the project guidelines
- Betty style guide contributors
- All peer reviewers and testers

## Technologies Used

<p align="left">
    <img src="https://img.shields.io/badge/C-a8b9cc?logo=&logoColor=black&style=for-the-badge" alt="C badge">
    <img src="https://img.shields.io/badge/GIT-f05032?logo=git&logoColor=white&style=for-the-badge" alt="Git badge">
    <img src="https://img.shields.io/badge/GITHUB-181717?logo=github&logoColor=white&style=for-the-badge" alt="GitHub badge">
    <img src="https://img.shields.io/badge/VALGRIND-purple?logo=v&logoColor=white&style=for-the-badge" alt="Valgrind badge">
    <img src="https://img.shields.io/badge/VIM-019733?logo=vim&logoColor=white&style=for-the-badge" alt="VIM badge">
</p>

## License

This project is part of the Holberton School curriculum. All rights reserved.

---

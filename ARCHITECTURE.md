# Architecture
This document will hold...
- More details on specific areas of interest of the shell.
- A list of some of the architecture decision records taken during development process.

## Architecture details 

### Key Functions @FIXME CHECK

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

### Input parsing
The input is read with getline(), trimmed of its newline then split as chunks using spaces/tabs as boundaries.  
No further process is applied.

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

### Command execution and exit
Command is runned through a childprocess to isolate it, guaranteeing continuity of service for the shell.
Each time a non builtin command is found and runned, its exit code is stored.
When user decides to exit through shortcut or "exit" builtin command, it exits with that same code.
This allows user to afterwards be able to read/retrieve that exit code in its "usual shell" with command `echo $?`.

Note that to allow that without completely changing our original code structure which had exit builtin returning -1,
  we used a technique suggested by AI to "encode the information". Basically the idea was turning from "returning -1 to signal termination request" to "return negative number to signal termination request". Then "reversing the sign and cancelling the -1" in main just before exiting.
This was a simple and efficient enough way to resolve our problem without rewriting everything. However it works only because we have complete control over the calls flow and functions prototypes so we know exactly how the builtin return was "computed" therefore how to "reverse" it.
So it is probably not the most maintainable/extensive paradigm for this specific functional aspect.

## Architecture Decision Records

### ADR-001 (2026-04-14) - Non-support of execution with arguments in v1.
- **Context**: main execution level, having program launched with arguments (ex "./hsh ls -l /tmp")
- **Options**: 1: "support" this way of running (requiring extra processing, not in mandatory scope). 2. Do not support.
- **Decision**: WILL NOT SUPPORT in v1 as this is an awkward way to run an interactive shell and would require extra effort.  
- **Consequences**: This kind of program execution will simply ignore those arguments in V1. Decision will be challenged if we have time left before deadline.

### ADR-002 (2026-04-14) - Non-check of PATH in main() process.
- **Context**: main execution level, considering that environment variables are provided.
- **Options**: 1: Immediately check if a PATH variable is present. 2. Delegate to the "search command" subpart.
- **Decision**: Will delegate to search command as it is not strictly speaking required for shell to work (just makes it restrictive in behaviour since meaning user needs to put absolute path).
- **Consequences**: Main() will just need to ensure that its argument envp is propagated as needed to reach the program charged to drill it for PATH.

### ADR-003 (2026-04-14) - Making the main loop "while TRUE then BREAK when readline ends/fail"
- **Context**: Function dedicated to managing the interactive mode loop.
- **Options**: 1. Make a first read with getline then "read while getline return different from -1". 2. Make a "always true loop" and manually break as soon as getline returns -1.
- **Decision**: Went for the "WHILE 1" approach. 
- **Consequences**: it is slightly less rigorous conceptually but has the merit of simplyfing code (no "first initializing outside of loop"). Especially since it will work the same with a non-interactive run (first loop cycle getline will read what was piped into it stopping at newline, second cycle will read nothing so will stop run).

### ADR-004 (2026-04-14) - Making the loop manager function return parsed line instead of getline's return code
- **Context**: Function dedicated to managing the interactive mode loop.
- **Options**: 1. "Bubble up" getline return directly. 2. Return the read string or NULL if getline return is -1.
- **Decision**: Went for the return read line directly.
- **Consequences**: it is easier in terms of architecture and normally does not change anything in logical architecture as long as main() properly "cleans and exit" when getting "NULL string" as if it had received (-1 return code) in the other approach.

### ADR-005 (2026-04-14) - Creating an intermediate orchestrator which just handles information passthrough.
- **Context**: Architecture of the logic from "I got some line to examine" to "I finished running command if it existed".
- **Options**: 1. Try to make an all-in-one. 2. Try to make separate functions called in main. 3. Have a function which just calls the separate ones.
- **Decision**: Went for the 3. Intermediate orchestrator because 1 could not fit ever in 40 lines and 2 was barely better.
- **Consequences**: The intermediate process will just hold the "results" of the "level 3 ones" meaning "array of tokens", "command fullpath" and "return codes of subfunctions". In particular this function won't make any string duplication.

### ADR-006 (2026-04-14) - Not requiring pointer of tokens array as argument for tokenizers.
- **Context**: function dedicated to the parsing and split of a string into an array of subparts.
- **Options**: 1: Have caller provide the adress of the strings array pointer. 2. Don't request anything and juste create+return the array.
- **Decision**: Went for 2 as... a) Option 1 would have required a "triple pointer" argument not very readable b) Ultimately it didn't bring anything since the pointer would have been provided empty.
- **Consequences**: Tokenize_string directly creates the array in its body (so "start as owner") BUT entirely delegates the management of the array and subpointers to caller by returning it.

### ADR-007 (2026-04-15) - Making tokenizer flexible on delimiters set.
- **Context**: function dedicated to the parsing and split of a string into an array of subparts.
- **Options**: 1: Just have "fixed set" to use. 2. Always require caller to provide the set. 3. Allow caller to provide but have a sensible fallback.
- **Decision**: Went for 3 as... a) Option 1 was fine as long as only one use-case, but exchanges with peers showed other potential use-cases. b) Refactor to support providing delimiters AND set a fallback was very minor, high benefit/effort ratio.
- **Consequences**: Tokenize_string has now adjustable behaviour depending on a "delimiters" character set being or not provided by caller.

### ADR-008 (2026-04-15) - Putting tokenize_string in own file "tokenizers.c"
- **Context**: function dedicated to the parsing and split of a string into an array of subparts.
- **Options**: 1: Just keep it in main. 2. Put it in "generic toolbox file" like utils.c or tools.c 3. Put in dedicated file tokenizer(s).c
- **Decision**: Went for 3 as... a) Per previous decision it became a potentially reusable function b) Current implementation is too big for betty so will need to be split in smaller functions YET only one "public facing" function is needed.
- **Consequences**: Tokenize_string has been set in a "tokenizers.c" file (name plural in case we develop variants of behaviours).

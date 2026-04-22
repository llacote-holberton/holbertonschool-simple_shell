# Architecture
This document will hold...
- More details on specific areas of interest of the shell.
- A list of some of the architecture decision records taken during development process.

## Architecture details 

### Key Functions

| File            | Function              | Description                                                                  |
|-----------------|-----------------------|------------------------------------------------------------------------------|
| `main.c`        | `main()`              | Main loop: prompt, read input, process commands                              |
| `main.c`        | `process_input()`     | Orchestrator from "I have some line as input" to "I runned command if found" |
| `builtins.c`    | `execute_builtin()`   | Checks and executes built-in commands                                        |
| `_getenv.c`     | `_getenv.c()`         | Wrapper to read a specific environment var from array given by main().       |
| `tokenizers.c`  | `tokenize_string()`   | Splits string into an array of substrings using a charset as delimiter(s).   |
| `path.c`        | `get_cmd_fullpath()`  | Try to find valid executable file from provided substring.                   |
| `executioner.c` | `execute_command()`   | Spawns a subprocess to execute command and propagates back its exit code.    |

### Inner Helper functions
Those are functions only called from within the file they are part of. Usually only by one function.
Therefore not declared in header file.
This was an architecture design we choose to try and implement some form of isolation.

| File            | Function                   | Description                                                                  |
|-----------------|----------------------------|------------------------------------------------------------------------------|
| `main.c`        | `get_input_line()`         | Reads user input with library func getline, trimming newline before pushing. |
| `main.c`        | `free_variables()`         | Frees the memory used for input processing (written to avoid duped code).    |
| `builtins.c`    | `builtin_exit()`           | Handles exit command (sets an "internal signal" so main exits afterwards).   |
| `builtins.c`    | `builtin_env()`            | Handles env command (prints all known environment variables line by line).   |
| `tokenizers.c`  | `get_default_delimiters()` | Provides sensible fallback (spaces/tabs/newlines) if no delimiters given.    |
| `tokenizers.c`  | `count_tokens()`           | Traverses a copy of string to split to predetermine size of final array.     |
| `path.c`        | `check_direct_path()`      | Check if substring matches an existing and executable absolute/relative path.|
| `path.c`        | `build_full_path()`        | Tries to reconstruct an absolute path to check as valid (or not) command.    |
| `path.c`        | `search_in_path()`         | Uses the previous to traverse dirs extracted from PATH and search command.   |

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

### ADR-009 (2026-04-16) - Allowing tokenizer to mutate the original input string.
- **Context**: Tokenization process of the user input string using `strtok`.
- **Options**: 1. Duplicate the input string before parsing to preserve the original. 2. Pass the original string to `strtok` directly.
- **Decision**: Went for 2. We allow `strtok` to insert null-bytes (`\0`) directly into the original input buffer.
- **Consequences**: Saves memory overhead by avoiding unnecessary `malloc` operations for string duplication. The original raw string is permanently mutated, but since we only need the extracted tokens for the rest of the execution pipeline, this is an acceptable and optimized trade-off.

### ADR-010 (2026-04-17) - Retaining the 3rd argument `envp` in main despite strict POSIX norms.
- **Context**: Accessing the system's environment variables to pass them to functions (like PATH resolution or `execve`).
- **Options**: 1. Use the strict POSIX compliant global variable `extern char **environ`. 2. Rely on the common extension `int main(int argc, char **argv, char **envp)`.
- **Decision**: Went for 2. We use the 3rd argument `envp` and propagate it manually.
- **Consequences**: While technically a historical extension rather than strict POSIX C, it is universally supported on Unix systems (Linux, macOS, WSL). It fits the project's pedagogical requirements and enforces rigorous pointer propagation throughout our function tree.

### ADR-011 (2026-04-17) - Hiding specific built-in functions behind a generic dispatcher.
- **Context**: Architecture of built-in commands management and execution (like `env`, `exit`).
- **Options**: 1. Expose each built-in function (e.g., `builtin_exit`, `builtin_env`) directly to the main orchestrator. 2. Keep them private and expose only a single dispatcher function (`execute_builtin`).
- **Decision**: Went for 2. Built-ins won't be publicly exposed.
- **Consequences**: It implements a strong separation of concerns by exposing a single "dispatcher". Making easier to add builtins later.

### ADR-012 (2026-04-17) - Postponing the "change directory" (cd) built-in implementation.
- **Context**: Defining the functional scope of the v1 release regarding navigation capabilities.
- **Options**: 1. Implement `cd`, which requires complex environment variable manipulation (`PWD`, `OLDPWD`) and `chdir` system calls. 2. Do not implement it for v1.
- **Decision**: WILL NOT SUPPORT in v1. 
- **Consequences**: Users will not be able to navigate directories within the shell. However, this prioritizes code quality, stability, and meeting the deadline for the requested functional scope before attempting complex feature additions.

### ADR-013 (2026-04-18) - Built-ins evaluation takes precedence over PATH resolution.
- **Context**: The order of operations when attempting to resolve a command typed by the user.
- **Options**: 1. Search the PATH first, then check built-ins. 2. Check built-ins first, then search the PATH if no match is found.
- **Decision**: Went for 2. Built-ins are always "searched" and evaluated first.
- **Consequences**: This follows the standard convention of real shells. It guarantees that our built-ins (like `exit`) will run properly even if a user places a malicious or identically named executable in their PATH, ensuring consistent core behavior.

### ADR-014 (2026-04-18) - Letting the caller handle memory freeing on "exit" command.
- **Context**: Handling the `exit` built-in command and the subsequent memory cleanup (allocated lines, tokens, etc.).
- **Options**: 1. Make the `exit` built-in free everything and call `exit()` directly. 2. Make the built-in only propagate an "exit request signal" back to the main loop.
- **Decision**: Went for 2. The built-in will just send a specific return code/signal to request termination.
- **Consequences**: This avoids ownership ambiguity. The orchestrator (or main loop), which allocated the memory in the first place, remains solely responsible for freeing it. It prevents double-frees, memory leaks, and keeps the code architecture linear and predictable.

### ADR-015 (2026-04-20) - Centralizing error handling into a single generic fprintf.
- **Context**: Managing error messages when a command fails to resolve or execute (e.g., permission denied, not found).
- **Options**: 1. Build a comprehensive error manager dealing with all specific `errno` cases. 2. Use a single generic `fprintf` outputting "cmd not found" for all failures.
- **Decision**: Went for 2. Replaced the error manager idea with a single `fprintf` call.
- **Consequences**: Keeps the architecture radically simple. While it sacrifices granularity on specific system errors, it perfectly matches the checker's expectations and validation scope without bloating the codebase.

### ADR-016 (2026-04-21) - Bubbling back computed standard codes for interrupted child processes.
- **Context**: Handling the exit status of a child process (command execution) when it is forcefully interrupted by a signal (e.g., segmentation fault, kill).
- **Options**: 1. Return a generic error code (like 1 or -1). 2. Compute and return the standard shell interrupted code (typically 128 + signal number).
- **Decision**: Went for 2. The executioner will compute the specific code and bubble it back.
- **Consequences**: This adheres to the unofficial POSIX standard for shell error codes, making our shell behaviour more intuitive for users (at least those who already used other shells or know about that standard).

### ADR-017 (2026-04-21) - Using an encoding/decoding system for propagating execution return codes.
- **Context**: Passing the exit status of executed commands back through multiple layers of functions to reach the main loop.
- **Options**: 1. Use global variables. 2. Add pointer arguments to all function signatures to carry the status. 3. Encode/decode the status directly within the function return values.
- **Decision**: Went for 3. Use an encoding/decoding system via return integers.
- **Consequences**: Completely avoids global variables (which are forbidden) while preventing a massive, risky overhaul of existing function signatures. The orchestrator simply decodes the return value to extract the actual status code.

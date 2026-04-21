# Testing
This document holds details on how we defined our plan to stress test our program against as many different (edge-)cases as possible to check that it behaved as intended in as many different situations as possible without either breaking or generating memory leaks.

Note that within this document we assume our shell has been compiled under the filename 'hsh' and user has it available in its current working directory.

## Memory Leak Testing
We made regular checks on potential memory leakages or ownership ambiguity by using the dedicated tool from the Valgrind testing suite.

```bash
# Interactive mode
valgrind --leak-check=full --show-leak-kinds=all ./hsh
# Then type commands and exit

## Non-interactive mode
echo "ls -latr /etc" | valgrind --leak-check=full --show-leak-kinds=all ./hsh
```

Expected result: **"All heap blocks were freed -- no leaks are possible"**

## Behaviour testing
Please note that although we tried to behave as closely as possible to "regular shells", our restricted set of features imply some difference in specific cases (for example in case of a permission problem we only show "not found" as for non-existent file).

### First step: simple manual tests
We started with simple isolated tests in non-interactive mode to help us develop features. Tests such as...
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
### Second step: comparing with a "professional shell"
We used Bourne shell (aka "bash") as a reference for comparison as it is the one most widely installed on various GNU/Linux distributions.
Making tests such as...
```bash
# Test your shell
echo "ls -l" | ./hsh > output_hsh.txt

# Test sh
echo "ls -l" | /bin/sh > output_sh.txt

# Compare
diff output_hsh.txt output_sh.txt
```

## Third step: simple automated Test Suite

How to create a simple script running various use-cases?  
Create a file `test_shell.sh` and fill it with following content (you can then adjust as you like):

<details>
<summary>(Click to view file content)</b></summary>


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

</details>

Run with:
```bash
chmod +x test_shell.sh
./test_shell.sh
```

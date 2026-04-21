# Testing
This document holds details on how we define our plan to stress test our program against as many different (edge-)cases as possible to check that it behaved as intended in as many different situations as possible without either breaking or generating memory leaks.

## Manual Testing

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

## Memory Leak Testing

Verify no memory leaks with Valgrind:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./hsh
# Then type commands and exit
```

Expected result: **"All heap blocks were freed -- no leaks are possible"**

## Comparison with /bin/sh

```bash
# Test your shell
echo "ls -l" | ./hsh > output_hsh.txt

# Test sh
echo "ls -l" | /bin/sh > output_sh.txt

# Compare
diff output_hsh.txt output_sh.txt
```

## Automated Test Suite

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

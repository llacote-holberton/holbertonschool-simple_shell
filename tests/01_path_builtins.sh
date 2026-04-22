#!/bin/bash

echo -e "\n----- Section 1: PATH: absolute vs relative vs builtins -----"

run_test "Test 1.1: absolute path '/bin/usr/ls', without arguments" "/usr/bin/ls"
run_test "Test 1.2: found in PATH 'ls', without arguments" "ls"
run_test "Test 1.3: wrong absolute path '/user/bin/ls', without arguments" "/user/bin/ls"
run_test "Test 1.4: wrong relative path './ls', without arguments" "./ls"
run_test "Test 1.5: 'nonexistent' command (should return 'not found')" "nonexistent"
run_test "Test 1.6: empty input (should just re-display prompt on a new line)" ""
run_test "Test 1.7: built-in 'env'" "env"
run_test "Test 1.8: explicit 'exit' (should quit shell)" "exit"
run_test "Test 1.9: multiple commands separated by newline character" "ls\npwd\nexit"
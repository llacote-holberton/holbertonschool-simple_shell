#!/bin/bash

echo -e "\n----- Section 6: NOT SUPPORTED (YET!) FEATURES -----"

run_test "Test 6.1: 'du -h --max-depth=1 | sort -hr | head -n 10' (requires piping support)" "du -h --max-depth=1 | sort -hr | head -n 10"
run_test "Test 6.2: 'find . -name \"*.c\"' (requires quotes escaping)" 'find . -name "*.c"'
run_test "Test 6.3: 'echo \"tomato sauce\" >> shopping_list.txt' (requires quotes escaping)" 'echo "tomato sauce" >> shopping_list.txt'
run_test "Test 6.4: 'echo \$PATH' (requires \$ interpolation)" 'echo $PATH'
run_test "Test 6.5: 'touch mynewfile && ls -l ./mynewfile' (requires parsing of && as command control)" "touch mynewfile && ls -l ./mynewfile"
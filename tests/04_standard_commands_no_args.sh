#!/bin/bash

echo -e "\n----- Section 4: COMMANDS: standard commands require arguments, WITHOUT arguments -----"

run_test "Test 4.1: 'man' (Manual pages of man)" "man "
run_test "Test 4.2: 'touch ' (requires a filename as argument!)" "touch "
run_test "Test 4.3: 'cp tests_files ' (destination is missing!)" "cp tests_files "
run_test "Test 4.4: 'ln -s tests_files ' (collision because only one arguments and path exists)" "ln -s tests_files "
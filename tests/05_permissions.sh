#!/bin/bash

echo -e "\n----- Section 5: PERMISSIONS: trying to call paths which are not 'valid commands' -----"

run_test "Test 5.0: 'ln -s /usr/bin/ls tests_files/my_symlink_to_ls ' (sets a symlink for subsequent tests)" "ln -s /usr/bin/ls tests_files/my_symlink_to_ls "
run_test "Test 5.1: './tests_files/my_symlink_to_ls' (should work as links to valid command)" "./tests_files/my_symlink_to_ls"
run_test "Test 5.2: './tests_files/noncrossable_directory_with_executable-inside/hello.sh' (should fail as dir not traversable)" "./tests_files/noncrossable_directory_with_executable-inside/hello.sh"
run_test "Test 5.3: './tests_files/' (trying to 'execute' a folder)" "./tests_files"
run_test "Test 5.4: './tests_files/readable_but_unexecutable_script.sh' (actual script but no execution permission)" "./tests_files/readable_but_unexecutable_script.sh"
run_test "Test 5.5: './tests_files/executable_but_unreadable_script.sh' (actual script but no read permission)" "./tests_files/executable_but_unreadable_script.sh"
run_test "Test 5.6: 'rm ./tests_files/my_symlink_to_ls' (removing link for next test run)" "rm ./tests_files/my_symlink_to_ls"
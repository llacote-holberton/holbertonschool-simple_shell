#!/bin/bash

echo -e "\n----- Section 2: COMMANDS: basic PATH supported commands which don't require arguments -----"

run_test "Test 2.1: 'pwd' (print working directory)" "pwd"
run_test "Test 2.2: 'free' (displays memory usage)" "free"
run_test "Test 2.3: 'date' (current date and time)" "date"
run_test "Test 2.4: 'whoami' (current username)" "whoami"
run_test "Test 2.5: 'id' (current username's id and group id)" "id"
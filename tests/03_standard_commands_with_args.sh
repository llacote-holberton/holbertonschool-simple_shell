#!/bin/bash

echo -e "\n----- Section 3: COMMANDS: standard commands require arguments, WITH arguments -----"

run_test "Test 3.1: 'cat tests_files/poem.txt' (Outputs a short poem)" "cat tests_files/poem.txt"
run_test "Test 3.2: 'uname -v' (shows info on OS)" "uname -v"
run_test "Test 3.3: 'ls -l -a -t -r /tmp' (all content of tmp sorted by most recent)" "ls -l -a -t -r /tmp"
run_test "Test 3.4: 'touch /tmp/newfile' (creates/updates date of empty file in /tmp)" "touch /tmp/newfile"

# Utilisation des apostrophes simples ' ' pour encadrer la commande permet d'envoyer les guillemets " "
run_test "Test 3.5: 'echo \"Hello\"' (shows Hello, with quotes in custom since no escaping)" 'echo "Hello"'

run_test "Test 3.6: 'mkdir ./tempfolder' (creates dir)" "mkdir ./hbt_tempfolder" "mkdir ./std_tempfolder"
run_test "Test 3.7: 'cp ./tests_files/file_to_copy.txt ./tempfolder/copied_file.txt'"    \
										"cp ./tests_files/file_to_copy.txt ./hbt_tempfolder/copied_file.txt" \
										"cp ./tests_files/file_to_copy.txt ./std_tempfolder/copied_file.txt"
run_test "Test 3.8: 'rm ./tempfolder/copied_file.txt'" "rm ./hbt_tempfolder/copied_file.txt" "rm ./std_tempfolder/copied_file.txt"
run_test "Test 3.9: 'rmdir ./tempfolder/' (removes folder)" "rmdir ./hbt_tempfolder/" "rmdir ./std_tempfolder/"

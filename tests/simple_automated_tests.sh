#!/bin/bash

HBT_SHELL="../hsh.out"
STD_SHELL="/bin/bash"

echo "===== SIMPLE SHELL TESTS ====="
echo "----- All comparison will be \'custom first\', official second -----"
echo "----- Tests based on assumption Bash and all core utils are installed. -----"

echo -e "\n----- Section 1: PATH: absolute vs relative vs builtins -----"

# Test 1.1: Absolute path ls
printf "\n\n----------------------------\n"
echo "Test 1.1: absolute path \'/bin/usr/ls\', without arguments"
echo "     -----     "
echo "/usr/bin/ls" | $HBT_SHELL
echo "     -----     "
echo "/usr/bin/ls" | $STD_SHELL
echo "----------------------------"

# Test 1.2: Relative path ls
printf "\n\n----------------------------\n"
echo "Test 1.2: found in PATH \'ls\', without arguments"
echo "     -----     "
echo "ls" | $HBT_SHELL
echo "     -----     "
echo "ls" | $STD_SHELL
echo "----------------------------"

# Test 1.3: Wrong path ls
printf "\n\n----------------------------\n"
echo "     -----     "
echo "Test 1.3: wrong absolute path \'/user/bin/ls\', without arguments"
echo "/user/bin/ls" | $HBT_SHELL
echo "     -----     "
echo "/user/bin/ls" | $STD_SHELL
echo "----------------------------"

# Test 1.4: Relative path
printf "\n\n----------------------------\n"
echo "Test 1.4: wrong relative path \'./ls\', without arguments"
echo "     -----     "
echo "./ls" | $HBT_SHELL
echo "     -----     "
echo "./ls" | $STD_SHELL
echo "----------------------------"

# Test 1.5: Command not found
printf "\n\n----------------------------\n"
echo "Test 1.5: 'nonexistent' command (should return 'not found')"
echo "     -----     "
echo "nonexistent" | $HBT_SHELL
echo "     -----     "
echo "nonexistent" | $STD_SHELL
echo "----------------------------"

# Test 1.6: Empty input
printf "\n\n----------------------------\n"
echo "Test 1.6: empty input (should just re-display prompt on a new line)"
echo "     -----     "
echo "" | $HBT_SHELL
echo "     -----     "
echo "" | $STD_SHELL
echo "----------------------------"

# Test 1.7: Built-in env
printf "\n\n----------------------------\n"
echo "Test 1.7: built-in \'env\'"
echo "     -----     "
echo "env" | $HBT_SHELL
echo "     -----     "
echo "env" | $STD_SHELL
echo "----------------------------"

# Test 1.8: Built-in exit
printf "\n\n----------------------------\n"
echo "Test 1.8: explicit 'exit' (should quit shell)"
echo "     -----     "
echo "exit" | $HBT_SHELL
echo "     -----     "
echo "exit" | $STD_SHELL
echo "----------------------------"

# Test 1.9: Multiple commands separated by new lines
printf "\n\n----------------------------\n"
echo "Test 1.9: multiple commands separated by newline character (must use -e to have newlines 'propagated')"
echo "     -----     "
echo -e "ls\\npwd\\nexit" | $HBT_SHELL
echo "     -----     "
echo -e "ls\npwd\nexit" | $STD_SHELL
echo "----------------------------"

echo -e "\n----- Section 2: COMMANDS: basic PATH supported commands which don't require arguments -----"

# Test 2.1: Print Working Directory
printf "\n\n----------------------------\n"
echo "Test 2.1: \'pwd\' (print working directory)"
echo "     -----     "
echo "pwd" | $HBT_SHELL
echo "     -----     "
echo "pwd" | $STD_SHELL
echo "----------------------------"

# Test 2.2: Free
printf "\n\n----------------------------\n"
echo "Test 2.2: \'free\' (displays memory usage)"
echo "     -----     "
echo "free" | $HBT_SHELL
echo "     -----     "
echo "free" | $STD_SHELL
echo "----------------------------"

# Test 2.3: Date
printf "\n\n----------------------------\n"
echo "Test 2.3: \'date\' (current date and time)"
echo "     -----     "
echo "date" | $HBT_SHELL
echo "     -----     "
echo "date" | $STD_SHELL
echo "----------------------------"


# Test 2.3: WhoamI
printf "\n\n----------------------------\n"
echo "Test 2.3: \'whoami\' (current username)"
echo "     -----     "
echo "whoami" | $HBT_SHELL
echo "     -----     "
echo "whoami" | $STD_SHELL
echo "----------------------------"

# Test 2.4: Id (current user's ID and group ID)
printf "\n\n----------------------------\n"
echo "Test 2.4: \'id\' (current username's id and group id)"
echo "     -----     "
echo "id" | $HBT_SHELL
echo "     -----     "
echo "id" | $STD_SHELL
echo "----------------------------"

echo -e "\n----- Section 3: COMMANDS: standard commands require arguments, WITH arguments -----"

# Test 3.1: Man
printf "\n\n----------------------------\n"
echo "Test 3.1: \'cat tests_files/poem.txt\' (Outputs a short poem)"
echo "     -----     "
echo "cat tests_files/poem.txt" | $HBT_SHELL
echo "     -----     "
echo "cat tests_files/poem.txt" | $STD_SHELL
echo "----------------------------"

# Test 3.2: Uname -v (prints Operating system infos)
printf "\n\n----------------------------\n"
echo "Test 3.2: \'uname -v\' (shows info on OS)"
echo "     -----     "
echo "uname -v" | $HBT_SHELL
echo "     -----     "
echo "uname -v" | $STD_SHELL
echo "----------------------------"

# Test 3.3: Ls -latr (prints all /tmp content sorted by most recent)
printf "\n\n----------------------------\n"
echo "Test 3.3: \'ls -l -a -t -r /tmp\' (all content of tmp sorted by most recent)"
echo "     -----     "
echo "ls -l -a -t -r /tmp" | $HBT_SHELL
echo "     -----     "
echo "ls -l -a -t -r /tmp" | $STD_SHELL
echo "----------------------------"

# Test 3.4: Touch (creates a file)
printf "\n\n----------------------------\n"
echo "Test 3.4: \'touch /tmp/newfile\' (creates/updates date of empty file in /tmp)"
echo "     -----     "
echo "touch /tmp/newfile" | $HBT_SHELL
echo "     -----     "
echo "touch /tmp/newfile" | $STD_SHELL
echo "----------------------------"

# Test 3.5: Echo
printf "\n\n----------------------------\n"
echo "Test 3.5: \'echo \"Hello\"\' (shows Hello, with quotes in custom since no escaping)"
echo "echo \"Hello\"" | $HBT_SHELL
echo "     -----     "
echo "echo \"Hello\"" | $STD_SHELL
echo "----------------------------"

# Test 3.6: Directory creation
printf "\n\n----------------------------\n"
echo "Test 3.6: \'mkdir ./tempfolder\' (creates dir)"
echo "mkdir ./hbt_tempfolder" | $HBT_SHELL
echo "     -----     "
echo "mkdir ./std_tempfolder" | $STD_SHELL
echo "----------------------------"

# Test 3.7: File copy
printf "\n\n----------------------------\n"
echo "Test 3.7: \'cp ./tests_files/file_to_copy.txt ./tempfolder/copied_file.txt\' (shows Hello)"
echo "cp ./tests_files/file_to_copy.txt ./hbt_tempfolder/copied_file.txt" | $HBT_SHELL
echo "     -----     "
echo "cp ./tests_files/file_to_copy.txt ./std_tempfolder/copied_file.txt" | $STD_SHELL
echo "----------------------------"

# Test 3.8: File removal
printf "\n\n----------------------------\n"
echo "Test 3.8: \'rm ./tempfolder/copied_file.txt\' (shows Hello)"
echo "rm ./hbt_tempfolder/copied_file.txt" | $HBT_SHELL
echo "     -----     "
echo "rm ./std_tempfolder/copied_file.txt" | $STD_SHELL
echo "----------------------------"

# Test 3.9: Dir removal
printf "\n\n----------------------------\n"
echo "Test 3.9: \'rm -f ./tempfolder/\' (removes folder)"
echo "     -----     "
echo "rmdir ./hbt_tempfolder/" | $HBT_SHELL
echo "     -----     "
echo "rmdir ./std_tempfolder/" | $STD_SHELL
echo "----------------------------"

echo -e "\n----- Section 4: COMMANDS: standard commands require arguments, WITHOUT arguments -----"

# Test 4.1: Man
printf "\n\n----------------------------\n"
echo "Test 4.1: \'man\' (Manual pages of man)"
echo "     -----     "
echo "man " | $HBT_SHELL
echo "     -----     "
echo "man " | $STD_SHELL
echo "----------------------------"

# Test 4.2: Touch (creates a file)
printf "\n\n----------------------------\n"
echo "Test 4.2: \'touch ' (requires a filename as argument!)"
echo "     -----     "
echo "touch " | $HBT_SHELL
echo "     -----     "
echo "touch " | $STD_SHELL
echo "----------------------------"

# Test 4.3: Copy (copies files or dirs)
printf "\n\n----------------------------\n"
echo "     -----     "
echo "Test 4.3: \'cp tests_files \' (destination is missing!)"
echo "cp tests_files " | $HBT_SHELL
echo "     -----     "
echo "cp tests_files " | $STD_SHELL
echo "----------------------------"

# Test 4.4: Link creation
printf "\n\n----------------------------\n"
echo "Test 4.4: \'ln -s tests_files \' (collision because only one arguments and path exists)"
echo "     -----     "
echo "ln -s tests_files " | $HBT_SHELL
echo "     -----     "
echo "ln -s tests_files " | $STD_SHELL
echo "----------------------------"

echo -e "\n----- Section 5: PERMISSIONS: trying to call paths which are not 'valid commands' -----"
# Test 5.0: Setting a symlink
printf "\n\n----------------------------\n"
echo "Test 5.0: \'ln -s /usr/bin/ls tests_files/my_symlink_to_ls \' (sets a symlink for subsequent tests)"
echo "     -----     "
echo "ln -s /usr/bin/ls tests_files/my_symlink_to_ls " | $HBT_SHELL
echo "     -----     "
echo "ln -s /usr/bin/ls tests_files/my_symlink_to_ls " | $STD_SHELL
echo "----------------------------"

# Test 5.1: Trying to call symbolic link to valid command
printf "\n\n----------------------------\n"
echo "Test 5.1: \'./tests_files/my_symlink_to_ls\' (should work as links to valid command)"
echo "     -----     "
echo "./tests_files/my_symlink_to_ls" | $HBT_SHELL
echo "     -----     "
echo "./tests_files/my_symlink_to_ls" | $STD_SHELL
echo "----------------------------"

# Test 5.2: Trying to call executable in a non-crossable directory
printf "\n\n----------------------------\n"
echo "Test 5.2: \'./tests_files/noncrossable_directory_with_executable-inside/hello.sh\' (should fail as dir not traversable)"
echo "     -----     "
echo "./tests_files/noncrossable_directory_with_executable-inside/hello.sh" | $HBT_SHELL
echo "     -----     "
echo "./tests_files/noncrossable_directory_with_executable-inside/hello.sh" | $STD_SHELL
echo "----------------------------"

# Test 5.3: Trying to call a folder as executable
printf "\n\n----------------------------\n"
echo "Test 5.3: \'./tests_files/\' (trying to 'execute' a folder)"
echo "     -----     "
echo "./tests_files" | $HBT_SHELL
echo "     -----     "
echo "./tests_files" | $STD_SHELL
echo "----------------------------"

# Test 5.4: Trying to call a readable but non executable file
printf "\n\n----------------------------\n"
echo "Test 5.4: \'./tests_files/readable_but_unexecutable_script.sh\' (actual script but no execution permission)"
echo "     -----     "
echo "./tests_files/readable_but_unexecutable_script.sh" | $HBT_SHELL
echo "     -----     "
echo "./tests_files/readable_but_unexecutable_script.sh" | $STD_SHELL
echo "----------------------------"

# Test 5.5: Trying to call an executable but non readable file
printf "\n\n----------------------------\n"
echo "Test 5.5: \'./tests_files/executable_but_unreadable_script.sh\' (actual script but no read permission)"
echo "     -----     "
echo "./tests_files/executable_but_unreadable_script.sh" | $HBT_SHELL
echo "     -----     "
echo "./tests_files/executable_but_unreadable_script.sh" | $STD_SHELL
echo "----------------------------"

# Test 5.6: Cleaning for next run
printf "\n\n----------------------------\n"
echo "Test 5.6: \'rm ./tests_files/my_symlink_to_ls\' (removing link for next test run)"
echo "     -----     "
echo "rm ./tests_files/my_symlink_to_ls" | $HBT_SHELL
echo "     -----     "
echo "rm ./tests_files/my_symlink_to_ls" | $STD_SHELL
echo "----------------------------"

echo -e "\n----- Section 6: NOT SUPPORTED (YET!) FEATURES -----"

# Test 6.1: Getting 10 biggest files in current dir with piping
printf "\n\n----------------------------\n"
echo "Test 6.1: \'du -h --max-depth=1 | sort -hr | head -n 10\' (requires piping support)"
echo "     -----     "
echo "du -h --max-depth=1 | sort -hr | head -n 10" | $HBT_SHELL
echo "     -----     "
echo "du -h --max-depth=1 | sort -hr | head -n 10" | $STD_SHELL
echo "----------------------------"

# Test 6.2: Finding C source files in subtree.
printf "\n\n----------------------------\n"
echo "Test 6.2: \'find . -name \"*.c\"\' (requires quotes escaping)"
echo "     -----     "
echo "find . -name \"*.c\"" | $HBT_SHELL
echo "     -----     "
echo "find . -name \"*.c\"" | $STD_SHELL
echo "----------------------------"

# Test 6.3: Appending text to an existing file.
printf "\n\n----------------------------\n"
echo "Test 6.3: \'echo \"tomato sauce\" >> shopping_list.txt\' (requires quotes escaping)"
echo "     -----     "
echo "echo \"tomato sauce\" >> shopping_list.txt" | $HBT_SHELL
echo "     -----     "
echo "echo \"tomato sauce\" >> shopping_list.txt" | $STD_SHELL
echo "----------------------------"

# Test 6.4: Reading variables.
printf "\n\n----------------------------\n"
echo "Test 6.4: \'echo \$PATH\' (requires $ interpolation)"
echo "     -----     "
echo "echo \$PATH" | $HBT_SHELL
echo "     -----     "
echo "echo \$PATH" | $STD_SHELL
echo "----------------------------"

# Test 6.5: Chaining commands
printf "\n\n----------------------------\n"
echo "Test 6.5: \'touch mynewfile && ls -l ./mynewfile\' (requires parsing of && as command control)"
echo "     -----     "
echo "touch mynewfile && ls -l ./mynewfile" | $HBT_SHELL
echo "     -----     "
echo "touch mynewfile && ls -l ./mynewfile" | $STD_SHELL
echo "----------------------------"

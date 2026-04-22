#!/bin/bash

# Configuration
#export HBT_SHELL="../hsh.out"
#export STD_SHELL="/bin/bash"

# Configurable version BUT can create a security risk use with caution!!
# ${VAR:-default} ": use fallback is empty" "- use fallback if undefined"
export HBT_SHELL="${HBT_SHELL:-../hsh.out}"
export STD_SHELL="${STD_SHELL:-/bin/bash}"

echo "===== SIMPLE SHELL TESTS ====="
echo "----- All comparison will be 'custom first', official second -----"
echo "----- Tests based on assumption Bash and all core utils are installed. -----"

# Centralized function to avoid useless repeat of "display wrapper"
# Argument 1: Test title and description
# Argument 2: Command(s) to inject (should have prefixed value if 3rd arg present)
# Argument 3: Optional "variant command" for tests altering data (like mkdir).
run_test() {
    local title="$1"
    local cmd_hbt="$2"
    # Using "non-existant fallback" to keep retrocompatibility.
    # -> Only tests requiring "variants" have to be modified to use 3rd arg.
    local cmd_std="${3:-$2}"

    printf "\n\n----------------------------\n"
    echo "$title"
    echo "     -----     "
    echo -e "$cmd_hbt" | $HBT_SHELL
    echo "     -----     "
    echo -e "$cmd_std" | $STD_SHELL
    echo "----------------------------"
}

# @important need to export function so subscripts can "know" it.
export -f run_test

# Using regex and character set to target subscripts and execute them in order.
for test_file in ./0[0-9]_*.sh; do
    if [ -f "$test_file" ]; then
        # Executes file content in the current shell instead of subshell.
        source "$test_file"
    fi
done

echo "===== END OF AUTOMATED TESTS ====="

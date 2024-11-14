#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Counter for tests
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Test function
run_test() {
    local name=$1
    local command=$2
    local expected_exit=${3:-0}

    ((TESTS_RUN++))
    printf "${BLUE}Test %d: ${YELLOW}%s${NC}\n" $TESTS_RUN "$name"
    echo "Command: $command"

    # Run command in minishell
    echo "$command" > test_input
    ../minishell < test_input > test_output 2>&1
    status=$?

    printf "${GREEN}Output:${NC}\n"
    cat test_output
    echo

    if [ $status -eq $expected_exit ]; then
        printf "${GREEN}✓ Status check passed (got: %d, expected: %d)${NC}\n" $status $expected_exit
        ((TESTS_PASSED++))
    else
        printf "${RED}✗ Status check failed (got: %d, expected: %d)${NC}\n" $status $expected_exit
        ((TESTS_FAILED++))
    fi
    echo "----------------------------------------"
}

# Start testing
echo -e "${BLUE}Starting Minishell Tests${NC}\n"

# CD Tests
echo -e "\n${BLUE}=== Testing CD ===${NC}\n"

run_test "CD Basic" "cd .." 0
run_test "CD Home" "cd" 0
run_test "CD Error" "cd nonexistentdirectory" 1
run_test "CD Too Many Args" "cd dir1 dir2" 1

# ECHO Tests
echo -e "\n${BLUE}=== Testing ECHO ===${NC}\n"

run_test "Echo Basic" "echo hello world" 0
run_test "Echo -n Option" "echo -n hello" 0
run_test "Echo Empty" "echo" 0
run_test "Echo Multiple Spaces" "echo hello      world" 0

# PWD Tests
echo -e "\n${BLUE}=== Testing PWD ===${NC}\n"

run_test "PWD Basic" "pwd" 0

# EXPORT Tests
echo -e "\n${BLUE}=== Testing EXPORT ===${NC}\n"

run_test "Export Basic" "export TEST=value" 0
run_test "Export Invalid" "export 2TEST=value" 1
run_test "Export No Value" "export TEST" 0

# UNSET Tests
echo -e "\n${BLUE}=== Testing UNSET ===${NC}\n"

run_test "Unset Basic" "unset PATH" 0
run_test "Unset Nonexistent" "unset NONEXISTENT" 0

# ENV Tests
echo -e "\n${BLUE}=== Testing ENV ===${NC}\n"

run_test "Env Basic" "env" 0

# EXIT Tests
echo -e "\n${BLUE}=== Testing EXIT ===${NC}\n"

run_test "Exit Basic" "exit" 0
run_test "Exit with Status" "exit 42" 42
run_test "Exit Invalid Status" "exit abc" 2

# Clean up
rm -f test_input test_output

# Print summary
echo -e "\n${BLUE}=== Test Summary ===${NC}"
echo -e "Total tests run: ${YELLOW}$TESTS_RUN${NC}"
echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Tests failed: ${RED}$TESTS_FAILED${NC}"

# Exit with failure if any tests failed
[ $TESTS_FAILED -eq 0 ] || exit 1
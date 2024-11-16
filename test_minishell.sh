#!/bin/bash

# Basic tests
echo "=== Basic Pipeline Tests ==="
./minishell << EOF
ls | wc -l
echo "hello world" | wc -w
cat Makefile | grep "SRC" | wc -l
EOF

# Multiple pipes
echo "=== Multiple Pipes Tests ==="
./minishell << EOF
ls | grep ".c" | wc -l
cat Makefile | grep "SRC" | sort | uniq
ls -la | grep "^d" | sort -r | head -n 3
EOF

# Builtin commands in pipeline
echo "=== Builtin Commands in Pipeline Tests ==="
./minishell << EOF
echo "Hello World" | grep "Hello"
pwd | wc -c
env | grep "PATH" | wc -l
EOF

# Error handling tests
echo "=== Error Handling Tests ==="
./minishell << EOF
ls nonexistentfile | wc -l
cat nonexistentfile | grep "test"
ls | nonexistentcommand
EOF

# Complex pipeline tests
echo "=== Complex Pipeline Tests ==="
./minishell << EOF
cat Makefile | grep "^[A-Z]" | sort | uniq -c
find . -type f -name "*.c" | xargs cat | wc -l
ls -R | grep ":" | tr -d ':' | sort -r
EOF

# Special characters handling
echo "=== Special Characters Tests ==="
./minishell << EOF
echo "Hello   World" | tr -s " " | wc -w
echo "Hello;World" | tr ";" " " | wc -w
echo "Hello > World" | tr ">" "|" | wc -w
EOF

# Mixed builtin and external commands
echo "=== Mixed Commands Tests ==="
./minishell << EOF
pwd | cat | wc -c
env | grep "HOME" | cut -d= -f2
echo "Testing" | cat | grep "Test" | wc -w
EOF
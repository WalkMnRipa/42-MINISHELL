#!/bin/bash

echo "Testing basic commands..."
echo "ls"
echo "pwd"
echo "echo hello"

echo "Testing pipes..."
echo "ls | wc -l"
echo "ls -la | grep ^d | wc -l"

echo "Testing redirections..."
echo "echo 'test content' > test.txt"
echo "cat < test.txt"
echo "echo 'more content' >> test.txt"

echo "Testing builtins..."
echo "cd .."
echo "pwd"
echo "cd -"
echo "export TEST=value"
echo "env | grep TEST"
echo "unset TEST"

echo "Testing quotes and variables..."
echo 'echo "$HOME"'
echo "echo '$HOME'"
echo 'echo "mixed $HOME and $USER"'

echo "Testing error cases..."
echo "./nonexistent"
echo "cat nonexistentfile"

echo "Testing heredoc..."
echo "cat << EOF"
echo "some content"
echo "more content"
echo "EOF"

echo "exit"
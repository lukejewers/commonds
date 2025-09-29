#!/usr/bin/env bash

set -xe

if [ -z "$1" ]; then
    echo "Error: No source file specified"
    echo "Usage: $0 <command_name> [args...]"
    exit 1
fi

command_name="$1"
shift

mkdir -p ./bin

# build the binary
gcc -Wall -Wextra -o bin/"$command_name" src/"$command_name".c

# run with all remaining arguments
./bin/"$command_name" "$@"

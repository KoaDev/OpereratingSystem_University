#!/bin/bash

N=$1
shift

# Reverse the remaining command line arguments
args=("$@")
for ((i=${#args[@]}-1; i>=0; i--)); do
    reversed_args+=("${args[i]}")
done

# Loop over every N-th parameter and print it
for ((i=N-1; i<${#reversed_args[@]}; i+=N)); do
    eval "echo -n \${reversed_args[$i]} ''"
done

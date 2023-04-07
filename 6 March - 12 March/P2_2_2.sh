#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 N [args...]"
  exit 1
fi

N=$1
shift

# Loop over every N-th parameter and print it
for ((i=N-1; i<$#; i+=N)); do
    eval "echo \${$((i+1))}"
done

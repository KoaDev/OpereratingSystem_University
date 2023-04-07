#!/bin/bash

# Check if the number of parameters is 1
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# Get the directory path from command line argument
dir=$1

# Display the given command line parameter
echo "---- Directory: $dir"

# Check if the directory exists
if [ ! -d "$dir" ]; then
    echo "---- Error: $dir is not a directory"
    exit 1
fi

# Function to list all files in the directory
function list_files {
    local files=( $(ls "$dir") )
    for f in "${files[@]}"; do
        echo "$f"
    done
    return ${#files[@]}
}

# Call list_files and store the number of files in ALLFILES variable
ALLFILES=$(list_files)

# Function to list all files in the directory that match the pattern
function list_pattern_files {
    local files=( $(ls "$dir" | grep "^f.*\.c$") )
    for f in "${files[@]}"; do
        echo "$f"
    done
    return ${#files[@]}
}

# Call list_pattern_files and store the number of files in FFILES variable
FFILES=$(list_pattern_files)

# Function to list all subdirectories in the directory
function list_subdirs {
    local dirs=( $(find "$dir" -type d -mindepth 1 -maxdepth 1) )
    for d in "${dirs[@]}"; do
        echo "$d"
    done
    return ${#dirs[@]}
}

# Call list_subdirs and store the number of subdirectories in DIR variable
DIR=$(list_subdirs)

# Create a subdirectory with name DIR
mkdir "${dir}/${DIR}"

# Create a file in the subdirectory named ALLFILES, with contents of variable FFILES
echo "$FFILES" > "${dir}/${DIR}/${ALLFILES}"

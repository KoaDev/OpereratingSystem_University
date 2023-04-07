# README

Welcome to this README! You will find below instructions and descriptions for each shell script exercise.

## Exercises

### P1_4_1

**Objective:** Display the line number of each line containing the string "number". Then, display the number of lines corresponding to that criteria, using the wc command.

### P1_4_2

**Objective:** Display the line number of each line containing the string "character." at the end of the line. Then, display the number of lines corresponding to that criteria, using the wc command.

### P1_4_3

**Objective:** Display the line number of each line containing numbers of two digits. Then, display the number of lines corresponding to that criteria, using the wc command.

### P2_2_1

**Objective:** Write a shell script named "ls_param.sh" that displays on the screen every N-th parameter from its command line, in the order they were specified. N is given as its first command line parameter. You may use the eval command. You can begin with the case N=1. Deal with the following cases:
- Display the result on a single line;

**Example:**
"ls_param.sh 1 p1 p2 p3 p4 p5
p1 p2 p3 p4 p5
ls_param.sh 2 p1 p2 p3 p4 p5
p2 p4"


### P3_4_files

**Objective:** Write a shell script, which is given (at execution) as a command line parameter the path to a directory and performs the following actions:
1. Check if the number of parameters given in the command line is 1. If not, terminate with a usage message.
2. Display the given command line parameter.
3. Check if its command line parameter corresponds to a directory. If not, terminate with an error message.
4. Execute a function list_files to display all the files in the given directory and calculate their number. Keep the computed number in a variable called ALLFILES.
5. Execute a function list_pattern_files to display all the files in the given directory, whose names start with "f" and terminate with ".c". Calculate their number and keep its value in a variable called FFILES.
6. Execute a function list_subdirs to display all sub-directories in the given directory and calculate their number. Keep it in a variable named DIR.
7. Create a subdirectory with the name ${DIR}.
8. Create in the ${DIR} subdirectory a file named ${ALLFILES}, whose contents must be the value of variable FFILES.

### OP_9_1

**Objective:** Write a shell script "ls_rev_param.sh" that displays on the screen every N-th parameter from its command line, considered in the reverse order than they were specified. N is given as its first command line parameter. You may use the eval command. You can begin with the case N=1. Deal with the following cases:
- Display the result on a single line;

### OP_9_2

**Objective:** Display the result on different lines.

**Example:**
"_rev_param.sh 1 p1 p2 p3 p4 p5
p5 p4 p3 p2 p1
ls_param.sh 2 p1 p2 p3 p4 p5
p4 p2"

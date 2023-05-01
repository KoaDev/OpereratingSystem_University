## P1_4:

**Problems**. Write in the command line the command to:

1. Display the line number of each line containing the string "number". Then, display the number of lines corresponding to that criteria, using the `wc` command.
2. Display the line number of each line containing the string "character." at the end of line. Then, display the number of lines corresponding to that criteria, using the `wc` command.
3. Display the line number of each line containing numbers of two digits. Then, display the number of lines corresponding to that criteria, using the `wc` command.

## P2_2 :

**Problem**. Write a shell script named "ls_param.sh" that displays on the screen every N-th parameter from its command line, in the order they were specified. N is given as its first command line parameter. You may use `eval` command. You can begin with the case N=1. Deal with the following cases:

- display the result on a single line;
- display the result on different lines.

Example (NOTE: in the following example, the character '>' represents the command prompt displayed by the shell!)

"ls_param.sh 1 p1 p2 p3 p4 p5
p1 p2 p3 p4 p5
ls_param.sh 2 p1 p2 p3 p4 p5
p2 p4"

## P3:

**Problem**. Write a shell script, which is given (at execution) as a command line parameter the path to a directory and performs the following actions:

1. Checks if the number of parameters given in the command line is 1. If not, terminate with an usage message.
2. Displays the given command line parameter.
3. Checks if its command line parameter corresponds to a directory. If not, terminates with an error message.
4. Executes a function `list_files` to display all the files in the given directory and calculate their number. Keep the computed number in a variable called `ALLFILES`.
5. Executes a function `list_pattern_files` to display all the files in the given directory, whose names start with "f" and terminate with ".c". Calculate their number and keep its value in a variable called `FFILES`.
6. Executes a function `list_subdirs` to display all sub-directories in the given directory and calculate their number. Keep it in a variable named `DIR`.
7. Create a subdirectory with the name `${DIR}`.
8. Create in the `${DIR}` subdirectory a file named `${ALLFILES}`, whose contents must be the value of variable `FFILES`.



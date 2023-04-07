## l03p1

uses bit-wise operation trying to generate all subsets of the set {0, 1, ..., N}, where N is given as a command line parameter. Correct compiling errors to make the program compile and execute correctly.

## l03p2

reads from keyboard (i.e. STDIN) two sets S1 and S2 as sorted arrays and computes the ratio between the intersection and union cardinal of the two sets. The calculated value is also called Jaccard similarity coefficient. Correct the program to perform the correct computation. Note: a set's cardinal is its number of elements, irrespective of their order.

## l03p3

reads an array of integers from the keyboard and computes the sum of prime elements. The program crashes during its execution. Read the Debugging C with VS Code guide and use the debugger in VSCode (or your preferred IDE) to debug the program and fix the bugs.

## l03p4

performs several types of operations (add, del, list, exit) on an array of data structures containing student information. The program functions correctly, though it contains memory leaks. Use the valgrind tool to identify the memory leaks and then fix the code to eliminate them by adding the necessary calls to free.
Compile the code using: `gcc -Wall -Werror l03p4.c -o l03p4 -g`
Then run it using: `valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./l03p4`
**Hint:** Try to find a way to send a list of inputs to the program so that you don't have to type the inputs every time you test the program.

## l03p5
`int insert(int *v1, int n1, int c1, int *v2, int n2, int pos);`
that inserts the v2 array of size n2 into the v1 array of current size n1 and maximum capacity c1 starting from position pos. The function returns 0 in case of success and -1 if the c1 capacity is not enough for the storing all the needed elements. In the latter case, the current state of the array v1 must not be affected.
**Example:** supposing v1 has a capacity c1 or 8 elements and its current contents being {1, 2, 3, 4, 5, 6}, so n1 = 6, and v2 = {10, 20} and pos = 3, the new v1's contents will be {1, 2, 3, 10, 20, 4, 5, 6}.

## l03p6

Write a C program that received a command line parameter, converts it to an unsigned integer (remember that command line parameters are NUL-terminated strings), interchange the first byte with the last one and the second with the one before the last and display the initial and resulted numbers. If the initial number is of the form abcd, where a, b, c and d each represent a byte, then the result should be dcba.

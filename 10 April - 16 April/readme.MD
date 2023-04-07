## P2

**Objective:** Write a C program that creates N threads (N is given as a command line argument of the program). Each thread executes the same function but with a unique ID received as parameter, starting from 1 and going up to N. Let the N threads display infinitely a message with their unique ID and the main thread waiting after all the created threads.

**Task:** Run in a (different) terminal the "htop" command. Pressing F5 (Tree View) you can see the threads of the process run by you.

**Note:** Test the execution of the program with N taking the following values: 2, 5, 10, 50, 100. Terminate the running process and its threads pressing CTRL+C in the terminal.

**Changes to implement:**
1. Change the code of the main thread such that it waits in a loop for input from the keyboard, staying in that loop until 'q' is pressed, then waits for the termination of the other created threads, displays the message "Thread main is finished." and terminates.
2. Change the other threads' function such that they also exit their own loop when the 'q' key is pressed (you can use a global variable visible to all threads), display the message "Thread TH_ID is finished." and terminates.

**Note:** If your solution is correctly implemented, you should not see the main thread's termination message before any other thread's termination message.

## P3

**Objective:** Implement one of the following two proposed variants.

**VARIANT 1:**
1. Write a C program that creates N threads (N is given as a command line argument of the program).
2. The program must compute the product of the first M natural numbers (M is also given as a command line argument of the program).

**VARIANT 2:**
1. Write a C program that creates N threads (N is given as a command line argument of the program).
2. The program must find out the first prime numbers smaller than M (M is also given as a command line argument of the program).

**Task:** Test your program with different values for N for the same large value of M (e.g., in the range of hundreds of thousands or even millions) and detect the optimal number of threads to get the required result in minimum time. You can use the function `gettimeofday` (see its man page) to measure the execution time of a sequence of code.

**Note:** Use an algorithm that supports parallelism, even if not the optimal one.

## P4

**Objective:** Write a C program that simulates the functionality of a concurrent server with the following functionality:
1. Creates a thread (called control thread) that waits in a loop for keyboard input, until 'x' is pressed.
2. Creates a thread (called connection thread) that waits in a loop for new "client" requests.
3. The connection thread creates periodically (e.g., every N seconds/milliseconds; you can use sleep/usleep functions) a new client thread to simulate the handling of a newly received client request.

**Client thread function:**
1. Display the message "[Client thread ID] Starting ..." containing the thread's ID, immediately after entering its function.
2. Sleep for a while (a random number of seconds or milliseconds).
3. Display the message "[Client thread ID] Ending ..." before exiting its function.

**Task:** When the 'x' key is pressed, the whole process must terminate, but not before the already running client threads terminate. As a result, you have to see on the screen the ending messages corresponding to all starting messages already displayed, i.e., all the started client threads must terminate normally. In the same time

## P5

**Objective:** Modify the previous problem's solution, such that new N client threads are created when N is introduced from the keyboard (N is a number) and let the newly created threads execute an infinite loop doing nothing but just using the processor (the simplest "useless" method could be `while(1);`).

**Task:** Note the way the thread waiting for input from keyboard (i.e., the I/O thread) reacts to user's actions when the number of client threads (CPU-bound) increases. Could you find a way to measure the reaction time of the I/O thread?

## P6

**Objective:** Problem number 6 from the lab text.

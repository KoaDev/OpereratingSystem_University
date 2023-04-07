**create-a-new-process**:

Change the `create-a-new-process.c` program such that to make distinction between the messages marked with "[---]" (lines 26 and 54), when they are displayed by the parent and child, using the PID of each process.

**process-hierarchy**:

Use the `fork` system call create the process hierarchy drawn on the blackboard. Use the `getpid` and `getppid` functions to illustrate the hierarchy and use the `wait/waitpid` syscalls to be sure the parent processes do not terminate before their children. We recommend you using the following code pattern when creating each child, i.e. for every call to `fork()`:
```c
pid_t pid = fork();
switch (pid) {
  case -1:
    // error case
    perror("Cannot create a new child");
    exit(1);
  case 0:
    // child 
    //... does what it is requested ...
    break;
  default:
    // parent
    // ... does what it is requested ...
}
```



load-and-execute :

Modify the load-and-execute.c program, such that to be able to execute in its child process any command (e.g. ls, ps, who) received as a command line parameter. It is supposed that the executed commands do not receive in their turn any parameter. 
Modify the code further, such that to be possible to execute commands with arguments, like "cat /etc/passwd", "ls -l /". 
Modify the code such that to simulate the asynchronous (parallel) execution of the commands, like in case of terminating them with "&" in the command line.  



Client and server :

Write two C programs: one called client.c, the other one called server.c. The client program displays a prompt (like '>') and reads from the keyboard (STDIN) two integers and one of the characters ’+’ or ’–’ (for example something like this: "1 + 4"). The read numbers and operation sign are transmitted with the help of the "execl" system call as parameters to a child process, which executes the code resulting from the compilation of "server.c" and perform the requested operation. After the child (server) process finishes the operation, it transmits the result to its parent process (the client) with the help of the "exit" system call. The client process prints the result on the screen and resumes its steps in a loop until '#' is given as an operation. NOTE.The exist status is stored on just 8 bits, so the possible values to be specified are in the range 0-255 if you consider it an unsigned number or -128-127 if you consider it a signed number.



**load-and-execute**:

Modify the `load-and-execute.c` program, such that to be able to execute in its child process any command (e.g. ls, ps, who) received as a command line parameter. It is supposed that the executed commands do not receive in their turn any parameter.

Modify the code further, such that to be possible to execute commands with arguments, like "cat /etc/passwd", "ls -l /".

Modify the code such that to simulate the asynchronous (parallel) execution of the commands, like in case of terminating them with "&" in the command line.

**Client and server**:

Write two C programs: one called `client.c`, the other one called `server.c`. The client program displays a prompt (like '>') and reads from the keyboard (STDIN) two integers and one of the characters ’+’ or ’–’ (for example something like this: "1 + 4"). The read numbers and operation sign are transmitted with the help of the "execl" system call as parameters to a child process, which executes the code resulting from the compilation of "server.c" and perform the requested operation. After the child (server) process finishes the operation, it transmits the result to its parent process (the client) with the help of the "exit" system call. The client process prints the result on the screen and resumes its steps in a loop until '#' is given as an operation. NOTE: The exist status is stored on just 8 bits, so the possible values to be specified are in the range 0-255 if you consider it an unsigned number or -128-127 if you consider it a signed number.

**number-of-child_processes**:

The number of child processes. This problem is OPTIONAL and for extra lab bonuses, but of course we expect you to send only your personal solutions (idea sharing is not allowed). Modify the code below such that:

```c
int main(int argc, char **argv)
{
  int pid, steps, skip;
  
  if (argc != 3) {
    printf("Usage: %s <loop_steps> <skip_step>", argv[0]);
    exit(1);
  }
  sscanf(argv[1], "%d", &steps);
  sscanf(argv[2], "%d", &skip);
  for (i=1; i<=steps; i++) {
    pid = fork();
    if (pid == 0 && i % skip == 0)
      break;
  }
}
```
Let it create the number of processes created by the given code. 
Displays on the screen just ONE time the number of processes created by the given code, including the parent process, i.e. the process starting the execution of the main function.
You are not allowed to use any mathematical formula to display the required number, but only let the created process communicate in some way to get that number.  
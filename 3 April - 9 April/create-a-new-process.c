#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int var;
    int childPid;

    var = 10;

    printf("[Parent] var = %d\n", var);

    printf("[Parent] I am the only process up to here. I will create a new process.\n");

    childPid = fork();

    // Check if successfully created the child
    if (childPid < 0)
    { // if not, terminate the parent too
        perror("Error creating new process");
        exit(1);
    }

    // Here we separate between the code executed by the parent and child
    if (childPid > 0)
    { //parent
        printf("[Parent - PID %d] The pid of my child (value returned by fork) is: %d\n", getpid(), childPid);
        printf("[Parent - PID %d] My own pid is: %d\n", getpid(), getpid());

        printf("[Parent - PID %d ---] Message1. This message is displayed by the parent only\n", getpid());

        // Each process has its own var
        var = 11;
        printf("[Parent - PID %d] Before child ends: var = %d\n", getpid(), var);

        // wait for the child to terminate
        waitpid(childPid, NULL, 0);

        // Display again the var to see it is not changed by child
        printf("[Parent - PID %d] After child ends: var = %d\n", getpid(), var);

    }
    else
    { // child
        printf("[Child - PID %d] The value returned by fork is: %d\n", getpid(), childPid);
        printf("[Child - PID %d] My own pid is: %d\n", getpid(), getpid());
        printf("[Child - PID %d ---] Message1. This message is displayed by the child only\n", getpid());

        // Each process has its own var
        var = 12;
        printf("[Child - PID %d] var = %d\n", getpid(), var);
    }

    // Here parent and child meet again
    printf("[Parent - PID %d ---] Message2.  This message is displayed by both parent and child\n", getpid());
}

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10

int main(int argc, char **argv)
{
    int childPid;

    printf("[Parent] PID=%d ParentPID=%d\n", getpid(), getppid());

    // Create a new child
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
        printf("[Parent] ChildPID=%d\n", childPid);

        waitpid(childPid, NULL, 0);

        printf("[Parent] Terminate the execution\n");
    }
    else
    { // child
        printf("[Child] PID=%d ParentPID=%d\n", getpid(), getppid());

        if (argc < 2)
        {
            fprintf(stderr, "[Child] No command provided\n");
            exit(1);
        }

        // Allocate an array to hold the arguments for the command
        char *args[MAX_ARGS + 1] = {0};
        int i;
        for (i = 1; i < argc && i <= MAX_ARGS; i++)
        {
            args[i - 1] = argv[i];
        }

        // Simulate asynchronous execution
        int background = 0;
        if (i > 1 && strcmp(args[i - 2], "&") == 0)
        {
            background = 1;
            args[i - 2] = NULL;
        }

        // Execute the command
        execvp(args[0], args);

        // Putting code here makes sense only when execvp does not execute successfully
        perror("Error executing exec");
        printf("[Child] Terminate the execution\n");
        exit(1);
    }
}

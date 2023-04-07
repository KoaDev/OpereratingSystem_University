#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int pid, steps, skip;
    int num_processes = 1;  // start with 1 for the parent process
    
    if (argc != 3)
    {
        printf("Usage: %s <loop_steps> <skip_step>", argv[0]);
        exit(1);
    }
    
    sscanf(argv[1], "%d", &steps);
    sscanf(argv[2], "%d", &skip);

    for (int i = 1; i <= steps; i++)
    {
        pid = fork();
        if (pid == 0 && i % skip == 0)
        {
            num_processes++;
            break;
        }
        else if (pid > 0)
        {
            wait(NULL);  // wait for child to finish
        }
    }

    if (pid > 0)  // only parent process prints the number of processes
    {
        printf("Number of processes created: %d\n", num_processes);
    }

    return 0;
}
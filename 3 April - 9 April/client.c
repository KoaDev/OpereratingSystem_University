#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_LEN 100

int main()
{
    char input[MAX_INPUT_LEN];
    int num1, num2;
    char op;
    int result;

    while (1)
    {
        printf("> ");
        fflush(stdout);

        // Read input from user
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL)
        {
            break;
        }

        // Check for exit command
        if (strcmp(input, "#\n") == 0)
        {
            break;
        }

        // Parse input
        if (sscanf(input, "%d %c %d", &num1, &op, &num2) != 3)
        {
            fprintf(stderr, "Invalid input: %s\n", input);
            continue;
        }

        // Create child process
        pid_t pid = fork();
        switch (pid)
        {
            case -1:
                perror("Cannot create a new child");
                exit(1);
            case 0:
                // Execute server program
                printf("Executing server program...\n");
                char num1_str[20];
                char num2_str[20];
                snprintf(num1_str, 20, "%d", num1);
                snprintf(num2_str, 20, "%d", num2);
                execl("./server", "./server", num1_str, num2_str, &op, (char *) NULL);

                // Error case, should not reach this point
                fprintf(stderr, "Error executing server program\n");
                exit(1);
            default:
                // Wait for child process to finish and get result
                waitpid(pid, &result, 0);
                if (WIFEXITED(result))
                {
                    int exit_status = WEXITSTATUS(result);
                    printf("%d %c %d = %d\n", num1, op, num2, exit_status);
                }
                break;
        }
    }

    printf("Exiting...\n");
    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    printf("I am the root process (PID %d)\n", getpid());
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("Cannot create a new child");
                exit(1);
            case 0:
                printf("I am a child process (PID %d) with parent (PPID %d)\n", getpid(), getppid());
                if (i == 0) {
                    pid_t pid2 = fork();
                    switch (pid2) {
                        case -1:
                            perror("Cannot create a new child");
                            exit(1);
                        case 0:
                            printf("I am a grandchild process (PID %d) with parent (PPID %d)\n", getpid(), getppid());
                            break;
                        default:
                            waitpid(pid2, NULL, 0);
                            printf("I am the child process (PID %d) waiting for my grandchild (PID %d)\n", getpid(), pid2);
                            break;
                    }
                }
                break;
            default:
                waitpid(pid, NULL, 0);
                printf("I am the parent process (PID %d) waiting for my child (PID %d)\n", getpid(), pid);
                break;
        }
    }
    return 0;
}

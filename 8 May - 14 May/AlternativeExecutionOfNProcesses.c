#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void increment(int pid, long long MAX) {
    long long count = 0;
    for (count = 0; count <= MAX; count++) {
        printf("Process %d at step %lld\n", pid, count);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_processes>\n", argv[0]);
        exit(1);
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        fprintf(stderr, "Number of processes must be greater than 0\n");
        exit(1);
    }

    int fds[num_processes][2];
    for (int i = 0; i < num_processes; i++) {
        if (pipe(fds[i]) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            for (int j = 0; j < num_processes; j++) {
                if (j != i) {
                    close(fds[j][0]);
                }
                if (j != (i + 1) % num_processes) {
                    close(fds[j][1]);
                }
            }

            while (1) {
                read(fds[i][0], NULL, 1);
                increment(i + 1, 5);
                write(fds[(i + 1) % num_processes][1], NULL, 1);
            }
        }
    }

    for (int i = 0; i < num_processes; i++) {
        close(fds[i][0]);
        close(fds[i][1]);
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    return 0;
}

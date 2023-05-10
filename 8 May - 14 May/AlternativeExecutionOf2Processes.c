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

int main() {
    int fd1[2], fd2[2];
    pid_t p1, p2;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        perror("Pipe creation failed");
        exit(1);
    }

    p1 = fork();
    if (p1 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (p1 == 0) {
        close(fd1[1]);
        close(fd2[0]);
        while (1) {
            read(fd1[0], NULL, 1);
            increment(1, 5);
            write(fd2[1], NULL, 1);
        }
    } else {
        p2 = fork();
        if (p2 < 0) {
            perror("Fork failed");
            exit(1);
        } else if (p2 == 0) {
            close(fd1[0]);
            close(fd2[1]);
            while (1) {
                read(fd2[0], NULL, 1);
                increment(2, 5);
                write(fd1[1], NULL, 1);
            }
        } else {
            close(fd1[0]);
            close(fd2[0]);
            close(fd1[1]);
            close(fd2[1]);

            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}

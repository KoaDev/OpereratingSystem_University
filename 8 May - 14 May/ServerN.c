#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define PIPE_NAME "my_pipe"

void *handle_client(void *arg) {
    int *client_fd = (int *)arg;
    char buf[32];
    int a, b;
    char op;
    int result;

    while (1) {
        read(*client_fd, buf, sizeof(buf));

        sscanf(buf, "%d %c %d", &a, &op, &b);

        if (op == '+') {
            result = a + b;
        } else if (op == '-') {
            result = a - b;
        } else {
            break;
        }

        write(*client_fd, &result, sizeof(result));
    }

    close(*client_fd);
    free(client_fd);
    return NULL;
}

int main() {
    int fd;
    int client_id = 0;

    mkfifo(PIPE_NAME, 0666);

    while (1) {
        fd = open(PIPE_NAME, O_RDONLY | O_NONBLOCK);
        if (fd != -1) {
            int *client_fd = malloc(sizeof(int));
            *client_fd = fd;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, client_fd);
            pthread_detach(thread_id);

            client_id++;
        }
        usleep(100000);
    }

    unlink(PIPE_NAME);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define PIPE_NAME "my_pipe"

int main() {
    int fd;
    char buf[32];
    int a, b;
    char op;
    int result;

    mkfifo(PIPE_NAME, 0666);

    while (1) {
        fd = open(PIPE_NAME, O_RDONLY);
        read(fd, buf, sizeof(buf));
        close(fd);

        sscanf(buf, "%d %c %d", &a, &op, &b);

        if (op == '+') {
            result = a + b;
        } else if (op == '-') {
            result = a - b;
        } else {
            break;
        }

        fd = open(PIPE_NAME, O_WRONLY);
        write(fd, &result, sizeof(result));
        close(fd);
    }

    unlink(PIPE_NAME);

    return 0;
}

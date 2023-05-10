#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PIPE_NAME "my_pipe"

int main() {
    int fd;
    char buf[32];
    int result;

    while (1) {
        printf("Enter operation (e.g., 3 + 4, or x to exit): ");
        fgets(buf, sizeof(buf), stdin);

        if (buf[0] == 'x') {
            break;
        }

        fd = open(PIPE_NAME, O_WRONLY);
        write(fd, buf, strlen(buf) + 1);
        close(fd);

        fd = open(PIPE_NAME, O_RDONLY);
        read(fd, &result, sizeof(result));
        close(fd);

        printf("Result: %d\n", result);
    }

    return 0;
}

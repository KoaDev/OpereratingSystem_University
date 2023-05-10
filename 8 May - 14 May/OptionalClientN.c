#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080

struct file_transfer {
    uint32_t filesize;
    uint32_t chunksize;
    uint32_t filename_len;
    char filename[1];
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int client_fd;
    struct sockaddr_in server_addr;
    char buf[4096];
    struct file_transfer *ft = (struct file_transfer *)buf;

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    ft->filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    ft->chunksize = sizeof(buf) - sizeof(struct file_transfer);
    ft->filename_len = strlen(argv[1]);
    strcpy(ft->filename, argv[1]);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        exit(1);
    }

    send(client_fd, buf, sizeof(struct file_transfer) + ft->filename_len, 0);

    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, ft->chunksize, file)) > 0) {
        send(client_fd, buf, bytes_read, 0);
    }

    fclose(file);
    close(client_fd);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080

struct file_transfer {
    uint32_t filesize;
    uint32_t chunksize;
    uint32_t filename_len;
    char filename[1];
};

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    struct file_transfer *ft;
    char buf[4096];
    int bytes_received;
    FILE *file;

    while (1) {
        recv(client_fd, buf, sizeof(struct file_transfer), 0);
        ft = (struct file_transfer *)buf;

        if (ft->filename_len == 0) {
            break;
        }

        file = fopen(ft->filename, "wb");
        if (!file) {
            perror("Failed to open file");
            break;
        }

        uint32_t remaining = ft->filesize;
        while (remaining > 0) {
            uint32_t to_receive = remaining < ft->chunksize ? remaining : ft->chunksize;
            bytes_received = recv(client_fd, buf, to_receive, 0);
            fwrite(buf, 1, bytes_received, file);
            remaining -= bytes_received;
        }

        fclose(file);
    }

    close(client_fd);
    free(arg);
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        int *client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, client_fd_ptr);
        pthread_detach(thread_id);
    }

    close(server_fd);

    return 0;
}

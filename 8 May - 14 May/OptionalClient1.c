#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buf[32];
    int result;

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

    while (1) {
        printf("Enter operation (e.g., 3 + 4, or x to exit): ");
        fgets(buf, sizeof(buf), stdin);

        if (buf[0] == 'x') {
            break;
        }

        send(client_fd, buf, strlen(buf) + 1, 0);
        recv(client_fd, &result, sizeof
        (result), 0);

        printf("Result: %d\n", result);
    }

    close(client_fd);

    return 0;
}

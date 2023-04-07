#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    unsigned int num = atoi(input);

    printf("Initial number: %u\n", num);

    unsigned char *bytes = (unsigned char *)&num;

    // swap the bytes
    unsigned char tmp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = tmp;
    tmp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = tmp;

    printf("Resulted number: %u\n", num);

    return 0;
}

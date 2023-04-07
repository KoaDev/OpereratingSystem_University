#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Invalid arguments\n");
        exit(1);
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    char op = argv[3][0];
    int result;

    switch (op)
    {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }

    // Send result back to parent process
    exit(result);
}

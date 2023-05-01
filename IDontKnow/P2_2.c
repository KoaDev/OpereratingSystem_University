#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#pragma pack(push,1)
typedef struct _OPERATION {
    int no1;
    int no2;
    char operator;
} OPERATION;
#pragma pack(pop)


int write_operation(int fd, OPERATION *op, off_t offset) {
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek");
        return -1;
    }

    ssize_t bytes_written = write(fd, op, sizeof(OPERATION));
    if (bytes_written == -1) {
        perror("write");
        return -1;
    }

    return 0;
}

int read_operation(int fd, OPERATION *op, int offset, int count) {
    int ret = lseek(fd, offset * sizeof(OPERATION), SEEK_SET);
    if (ret < 0) {
        perror("lseek");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        ret = read(fd, op + i, sizeof(OPERATION));
        if (ret < 0) {
            perror("read");
            return -1;
        } else if (ret == 0) {
            printf("End of file reached.\n");
            return i;
        }
    }

    return count;
}

int perform_operations() {
    char input_file[256], output_file[256];
    printf("Enter the input file name: ");
    scanf("%s", input_file);
    printf("Enter the output file name: ");
    scanf("%s", output_file);

    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "w");

    if (fin == NULL) {
        printf("Error: Cannot open input file.\n");
        return -1;
    }

    if (fout == NULL) {
        printf("Error: Cannot open output file.\n");
        return -1;
    }

    OPERATION op;
    int num_ops = 0;

    while (fread(&op, sizeof(OPERATION), 1, fin) == 1) {
        int result;
        switch (op.operator) {
            case '+':
                result = op.no1 + op.no2;
                break;
            case '-':
                result = op.no1 - op.no2;
                break;
            case '*':
                result = op.no1 * op.no2;
                break;
            default:
                printf("Error: Invalid operator %c.\n", op.operator);
                return -1;
        }
        fprintf(fout, "%d %c %d = %d\n", op.no1, op.operator, op.no2, result);
        num_ops++;
    }

    fclose(fin);
    fclose(fout);

    printf("Performed operations on %d records and wrote results to %s.\n", num_ops, output_file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <filename> <offset> <count>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int offset = atoi(argv[2]);
    int count = atoi(argv[3]);

    OPERATION ops[count];

    // Generate some random operations
    srand(42);
    for (int i = 0; i < count; i++) {
        ops[i].no1 = rand() % 100;
        ops[i].no2 = rand() % 100;
        ops[i].operator = (rand() % 2 == 0) ? '+' : '-';
    }

    // Write the operations to file
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int ret = write_operation(fd, ops, offset);
    if (ret < 0) {
        printf("Error: failed to write to file\n");
        close(fd);
        return 1;
    }

    close(fd);

    // Read the operations from file and print them
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ret = read_operation(fd, ops, offset, count);
    if (ret < 0) {
        printf("Error: failed to read from file\n");
        close(fd);
        return 1;
    }

    for (int i = 0; i < ret; i++) {
        printf("Operation %d: %d %c %d\n", i, ops[i].no1, ops[i].operator, ops[i].no2);
    }

    close(fd);



    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int rett = perform_operations(fd);
    if (rett < 0) {
        printf("Error: failed to perform operations\n");
        close(fd);
        return 1;
    }

    close(fd);


    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check that the correct number of command-line arguments were provided
    if (argc != 4) {
        printf("Usage: %s file_name position string\n", argv[0]);
        return 1;
    }

    // Parse the command-line arguments
    char *file_name = argv[1];
    long position = atol(argv[2]);
    char *string = argv[3];
    size_t string_len = strlen(string);

    // Open the file for reading and writing
    FILE *file = fopen(file_name, "rb+");
    if (file == NULL) {
        printf("Error: Failed to open file '%s'\n", file_name);
        return 1;
    }

    // Move the file pointer to the specified position
    if (fseek(file, position, SEEK_SET) != 0) {
        printf("Error: Failed to set file position\n");
        fclose(file);
        return 1;
    }

    // Allocate a buffer to read the remaining file contents into
    size_t remaining_len = 0;
    long end_position = 0;
    char *remaining = NULL;
    if (fscanf(file, "%ms", &remaining) != EOF) {
        remaining_len = strlen(remaining);
        end_position = ftell(file) - remaining_len;
    } else {
        end_position = ftell(file);
    }

    // Write the string to the file at the specified position
    if (fwrite(string, sizeof(char), string_len, file) != string_len) {
        printf("Error: Failed to write string to file\n");
        fclose(file);
        return 1;
    }

    // Write the remaining file contents back to the file
    if (fwrite(remaining, sizeof(char), remaining_len, file) != remaining_len) {
        printf("Error: Failed to write remaining file contents to file\n");
        fclose(file);
        return 1;
    }

    // Clean up
    free(remaining);
    fclose(file);
    return 0;
}
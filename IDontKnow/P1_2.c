#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int get_line(int fd, char *line, int line_no, int max_length, int *line_length) {
    int current_line = 1;
    int pos = 0;
    char ch;
    *line_length = 0;

    // Read characters until the requested line or end of file
    while (current_line < line_no) {
        if (read(fd, &ch, 1) != 1) {
            // End of file reached before target line
            return -1;
        }
        if (ch == '\n') {
            // Newline character found, count as line
            current_line++;
        }
    }

    // Read characters of the requested line
    while (pos < max_length - 1 && read(fd, &ch, 1) == 1) {
        if (ch == '\n') {
            // End of line found, stop reading
            break;
        }
        line[pos++] = ch;
    }

    // Null-terminate the line
    line[pos] = '\0';
    *line_length = pos;

    if (pos == 0 && current_line < line_no) {
        // Empty line encountered, target line does not exist
        return -1;
    }

    return 0;
}

void reverse_lines(const char *src_filename, const char *dest_filename) {
    // Open source file
    FILE *src_file = fopen(src_filename, "r");
    if (!src_file) {
        printf("Error: could not open source file '%s'\n", src_filename);
        return;
    }

    // Open destination file
    FILE *dest_file = fopen(dest_filename, "w");
    if (!dest_file) {
        printf("Error: could not open destination file '%s'\n", dest_filename);
        fclose(src_file);
        return;
    }

    // Read lines from source file into a buffer
    char **lines = NULL;
    int num_lines = 0;
    int max_lines = 0;
    char line_buffer[1024];
    while (fgets(line_buffer, sizeof(line_buffer), src_file)) {
        if (num_lines >= max_lines) {
            max_lines = max_lines > 0 ? max_lines * 2 : 16;
            lines = realloc(lines, max_lines * sizeof(char *));
            if (!lines) {
                printf("Error: out of memory\n");
                fclose(src_file);
                fclose(dest_file);
                return;
            }
        }
        lines[num_lines] = strdup(line_buffer);
        if (!lines[num_lines]) {
            printf("Error: out of memory\n");
            fclose(src_file);
            fclose(dest_file);
            return;
        }
        num_lines++;
    }

    // Write lines from buffer into destination file in reverse order
    for (int i = num_lines - 1; i >= 0; i--) {
        fputs(lines[i], dest_file);
        free(lines[i]);
    }
    free(lines);

    // Close files
    fclose(src_file);
    fclose(dest_file);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <line number>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    int line_no = atoi(argv[2]);

    // Open file
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Error: could not open file '%s'\n", filename);
        return 1;
    }

    // Read target line
    char line[1024];
    int line_length;
    int status = get_line(fd, line, line_no, sizeof(line), &line_length);

    // Close file
    close(fd);

    if (status < 0) {
        printf("Error: could not read line %d from file '%s'\n", line_no, filename);
        return 1;
    }

    printf("Line %d: %s (length=%d)\n", line_no, line, line_length);
    return 0;
}

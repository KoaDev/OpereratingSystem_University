#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int total_file_size(char *dir_path);
int total_file_tree_size(char *dir_path);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    char *dir_path = argv[1];

    struct stat s;
    if (stat(dir_path, &s) != 0) {
        perror("stat");
        return 1;
    }

    if (!S_ISDIR(s.st_mode)) {
        printf("Error: '%s' is not a directory.\n", dir_path);
        return 1;
    }

    printf("Total size of files in directory '%s': %d bytes, %ld blocks\n", dir_path, total_file_size(dir_path), s.st_blocks);
    printf("Total size of files in directory tree '%s': %d bytes, %ld blocks\n", dir_path, total_file_tree_size(dir_path), s.st_blocks);

    return 0;
}

int total_file_size(char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat s;
    int total_size = 0;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &s) != 0) {
            perror("stat");
            closedir(dir);
            return 0;
        }

        if (S_ISREG(s.st_mode)) {
            total_size += s.st_size;
        } else if (S_ISDIR(s.st_mode)) {
            total_size += total_file_size(path);
        }
    }

    closedir(dir);
    return total_size;
}

int total_file_tree_size(char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat s;
    int total_size = 0;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &s) != 0) {
            perror("stat");
            closedir(dir);
            return 0;
        }

        if (S_ISREG(s.st_mode)) {
            total_size += s.st_size;
        } else if (S_ISDIR(s.st_mode)) {
            total_size += total_file_size(path);
            total_size += total_file_tree_size(path);
        }
    }

    closedir(dir);
    return total_size;
}

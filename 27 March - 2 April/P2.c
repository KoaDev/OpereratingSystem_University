#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int remove_files_in_dir(char *dir_path);
int remove_file_tree(char *dir_path);

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

    remove_file_tree(dir_path);

    return 0;
}

int remove_files_in_dir(char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (unlink(path) != 0) {
            perror("unlink");
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

int remove_file_tree(char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (remove_file_tree(path) != 0) {
                closedir(dir);
                return 1;
            }

            if (rmdir(path) != 0) {
                perror("rmdir");
                closedir(dir);
                return 1;
            }
        } else {
            if (unlink(path) != 0) {
                perror("unlink");
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int search_dir(char *dir_name, char *searched_name);
int search_tree(char *dir_name, char *searched_name);
int copy_file(char *src, char *dst);
int copy_dir(char *src, char *dst);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <search_directory> <searched_name> <destination_directory> <destination_extension>\n", argv[0]);
        return 1;
    }

    char *search_directory = argv[1];
    char *searched_name = argv[2];
    char *destination_directory = argv[3];
    char *destination_extension = argv[4];

    struct stat s;
    if (stat(search_directory, &s) != 0) {
        perror("stat");
        return 1;
    }

    if (!S_ISDIR(s.st_mode)) {
        printf("Error: '%s' is not a directory.\n", search_directory);
        return 1;
    }

    int result = search_tree(search_directory, searched_name);
    if (result) {
        char src[PATH_MAX];
        char dst[PATH_MAX];
        snprintf(src, sizeof(src), "%s/%s", search_directory, searched_name);
        snprintf(dst, sizeof(dst), "%s/%s.%s", destination_directory, searched_name, destination_extension);

        if (S_ISDIR(s.st_mode)) {
            copy_dir(src, dst);
        } else {
            copy_file(src, dst);
        }
    } else {
        printf("Element not found.\n");
    }

    return 0;
}

int search_dir(char *dir_name, char *searched_name) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, searched_name) == 0) {
            printf("Found: %s/%s\n", dir_name, searched_name);
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

int search_tree(char *dir_name, char *searched_name) {
    if (search_dir(dir_name, searched_name)) {
        return 1;
    }

    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        return 0;
    }

    int found = 0;
    char path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
            found = search_tree(path, searched_name);
            if (found) {
                break;
            }
        }
    }

    closedir(dir);
    return found;
}

int copy_file(char *src, char *dst) {
    FILE *src_file, *dst_file;
    char buf[BUFSIZ];
    size_t n;

    src_file = fopen(src, "rb");
    if (src_file == NULL) {
        perror("fopen");
        return 1;
    }
    dst_file = fopen(dst, "wb");
    if (dst_file == NULL) {
        perror("fopen");
        fclose(src_file);
        return 1;
    }

    while ((n = fread(buf, 1, sizeof(buf), src_file)) > 0) {
        fwrite(buf, 1, n, dst_file);
    }

    fclose(src_file);
    fclose(dst_file);
    printf("File copied: %s -> %s\n", src, dst);
    return 0;
}

int copy_dir(char *src, char *dst) {
    DIR *dir;
    struct dirent *entry;
    struct stat src_stat;
    char src_path[PATH_MAX], dst_path[PATH_MAX];
    if (stat(src, &src_stat) != 0) {
        perror("stat");
        return 1;
    }

    if (mkdir(dst, src_stat.st_mode) != 0) {
        perror("mkdir");
        return 1;
    }

    dir = opendir(src);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

        if (entry->d_type == DT_DIR) {
            copy_dir(src_path, dst_path);
        } else {
            copy_file(src_path, dst_path);
        }
    }

    closedir(dir);
    printf("Directory copied: %s -> %s\n", src, dst);
    return 0;
}
/*
Read two sets from keyboard. First two numbers provided
will be the number of elements within each set. Next,
you will provide the elements from each set.
Compute card(intersection(set1, set2)) and card(union(set1, set2))
for each set, and, after that,
card(intersection(set1, set2)) / card(union(set1, set2))
card(x) is the number of elements within set x
*/

#include <stdio.h>

#define SUCCESS 0
#define ERROR_READ 1
#define MAX_SET_SIZE 150

int read_set(int *set, int *size) {
    int i, sz;
    *size = 0;

    if (scanf("%d", &sz) != 1) {
        printf("Error reading size!\n");
        return ERROR_READ;
    }

    for (i = 0; i < sz; i++) {
        if (scanf("%d", &set[i]) != 1) {
            printf("Error reading element %d from set\n", i);
            return ERROR_READ;
        }
    }

    *size = sz;
    return SUCCESS;
}

float jaccard_similarity(int *set1, int *set2, int size1, int size2) {
    int i = 0, j = 0, common = 0, total = 0;

    while (i < size1 && j < size2) {
        if (set1[i] == set2[j]) {
            common++;
            i++;
            j++;
        } else if (set1[i] < set2[j]) {
            i++;
        } else {
            j++;
        }
        total++;
    }

    while (i < size1) {
        i++;
        total++;
    }

    while (j < size2) {
        j++;
        total++;
    }

    return (float) common / total;
}

int main(int argc, char **argv) {
    int set1[MAX_SET_SIZE], set2[MAX_SET_SIZE], size1, size2, res;

    res = read_set(set1, &size1);
    if (res != SUCCESS) {
        return res;
    }

    res = read_set(set2, &size2);
    if (res != SUCCESS) {
        return res;
    }

    float jaccard = jaccard_similarity(set1, set2, size1, size2);
    printf("Jaccard similarity coefficient: %.2f\n", jaccard);
    return SUCCESS;
}

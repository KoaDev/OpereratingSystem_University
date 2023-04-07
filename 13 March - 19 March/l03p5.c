#include <stdio.h>

/*
 * The function takes in the following arguments:
 * v1: Pointer to the first element of the array v1.
 * n1: The current size of the array v1.
 * c1: The maximum capacity of the array v1.
 * v2: Pointer to the first element of the array v2.
 * n2: The size of the array v2.
 * pos: The position in v1 where the elements of v2 are to be inserted.
 */

int insert(int *v1, int n1, int c1, int *v2, int n2, int pos) {
    if (n1 + n2 > c1) {
        return -1; // Capacity not enough
    }
    // Move existing elements after the insertion point to create space for v2
    for (int i = n1 - 1; i >= pos; i--) {
        v1[i + n2] = v1[i];
    }
    // Copy the elements of v2 into the newly created space
    for (int i = 0; i < n2; i++) {
        v1[pos + i] = v2[i];
    }
    return 0; // Success
}

int main() {
    int v1[8] = {1, 2, 3, 4, 5, 6};
    int n1 = 6;
    int c1 = 8;
    int v2[2] = {10, 20};
    int n2 = 2;
    int pos = 3;

    int result = insert(v1, n1, c1, v2, n2, pos);

    if (result == 0) {
        for (int i = 0; i < n1 + n2; i++) {
            printf("%d ", v1[i]);
        }
    } else {
        printf("Capacity not enough");
    }

    return 0;
}

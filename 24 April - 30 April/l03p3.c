#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

// fixed function to check if a number is prime
int isPrime(int number)
{
    int i;
    if (number < 2) {
        return FALSE;
    }
    for(i = 2; i <= number/2; i++){
        if(number % i == 0){
            return FALSE;
        }
    }
    return TRUE;
}

void computeSum(int *array, int size, int *sum)
{
    int i, tempSum = 0;
    for(i=0; i<size; i++){
        if(isPrime(array[i]) == TRUE){
            tempSum += array[i];
        }
    }
    *sum = tempSum;
}

int readArray(int *size, int *array)
{
    int i, sz;

    printf("Size: \n");
    if (scanf("%d", &sz) != 1){ // added & before sz to read the value into the variable
        printf("Error reading size!\n");
        return FALSE;
    }

    printf("Array:\n");
    for(i = 0; i < sz; i++){
        if(scanf("%d", &array[i]) != 1){ // added & before array[i] to read the value into the array element
            printf("Error reading element %d!\n",i);
            return  FALSE;
        }
    }

    *size = sz; // added & before size to modify the variable
    return TRUE;
}

int main()
{
    int sum,size;
    int *array;

    array = (int *)malloc(sizeof(int)*150);

    if(readArray(&size, array) == TRUE){ // removed & before array to pass the array pointer
        computeSum(array, size, &sum); // added & before sum to pass the address of the variable
        printf("%d", sum); // added format specifier for integer
    }

    free(array); // added freeing the allocated memory
    return 0;
}

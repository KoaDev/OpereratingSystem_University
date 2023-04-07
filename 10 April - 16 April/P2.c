#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Declare a global variable to signal when threads should exit their loops
volatile bool exit_threads = false;

// Thread function
void* print_thread_id(void* arg) {
    int id = *((int*) arg);

    while (!exit_threads) {
        printf("Thread %d\n", id);
        sleep(1); // Sleep for a while to avoid flooding the console
    }

    printf("Thread %d is finished.\n", id);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);

    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    int* thread_ids = malloc(sizeof(int) * num_threads);

    // Create N threads
    for (int i = 0; i < num_threads; ++i) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, print_thread_id, &thread_ids[i]);
    }

    // Wait for input from the keyboard and exit when 'q' is pressed
    char ch;
    do {
        ch = getchar();
    } while (ch != 'q');

    // Signal the threads to exit their loops
    exit_threads = true;

    // Wait for the termination of the other created threads
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Thread main is finished.\n");

    free(threads);
    free(thread_ids);

    return 0;
}

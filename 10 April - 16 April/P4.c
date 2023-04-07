#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

volatile bool exit_server = false;
volatile bool exit_connection_thread = false;

void* control_thread_func(void* arg) {
    char ch;
    do {
        ch = getchar();
    } while (ch != 'x');

    exit_server = true;
    return NULL;
}

void* client_thread_func(void* arg) {
    int id = *((int*) arg);
    printf("[Client thread %d] Starting ...\n", id);

    int sleep_time = rand() % 5 + 1; // Sleep for a random duration between 1 and 5 seconds
    sleep(sleep_time);

    printf("[Client thread %d] Ending ...\n", id);
    free(arg);
    return NULL;
}

void* connection_thread_func(void* arg) {
    int client_id = 1;

    while (!exit_server) {
        usleep(500000); // Sleep for 500 milliseconds

        if (!exit_connection_thread) {
            int* client_id_ptr = malloc(sizeof(int));
            *client_id_ptr = client_id;
            pthread_t client_thread;
            pthread_create(&client_thread, NULL, client_thread_func, client_id_ptr);
            pthread_detach(client_thread);
            ++client_id;
        }
    }

    exit_connection_thread = true;
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t control_thread;
    pthread_t connection_thread;

    pthread_create(&control_thread, NULL, control_thread_func, NULL);
    pthread_create(&connection_thread, NULL, connection_thread_func, NULL);

    pthread_join(control_thread, NULL);

    while (!exit_connection_thread) {
        usleep(100000); // Sleep for 100 milliseconds
    }

    return 0;
}
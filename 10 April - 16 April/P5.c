#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

volatile bool exit_server = false;
volatile bool exit_connection_thread = false;

void* control_thread_func(void* arg) {
    struct timeval start, end;
    double reaction_time;

    while (!exit_server) {
        int n;
        gettimeofday(&start, NULL);
        scanf("%d", &n);

        if (n == -1) {
            exit_server = true;
        } else {
            for (int i = 0; i < n; ++i) {
                pthread_t client_thread;
                pthread_create(&client_thread, NULL, client_thread_func, NULL);
                pthread_detach(client_thread);
            }
        }

        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        reaction_time = seconds + microseconds * 1e-6;
        printf("Reaction time: %.6f seconds\n", reaction_time);
    }

    return NULL;
}

void* client_thread_func(void* arg) {
    while (1); // Infinite loop using the processor
    return NULL;
}

int main() {
    pthread_t control_thread;

    pthread_create(&control_thread, NULL, control_thread_func, NULL);

    pthread_join(control_thread, NULL);

    return 0;
}

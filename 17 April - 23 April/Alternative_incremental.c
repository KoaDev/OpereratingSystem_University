#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

long long count = 0;
sem_t semaphore1, semaphore2;

void increment(void *th_id) {
    int i = 0;
    while (i < 100) {
        if ((intptr_t)th_id == 0) {
            sem_wait(&semaphore1);
        } else {
            sem_wait(&semaphore2);
        }
        long long aux;
        aux = count;
        aux++;
        printf("count=%lld from thread %d\n", count, (int)(intptr_t)th_id + 1);
        count = aux;
        i++;

        if ((intptr_t)th_id == 0) {
            sem_post(&semaphore2);
        } else {
            sem_post(&semaphore
            1);
        }
    }
    pthread_exit(NULL);
}

int main() {
    int num_threads = 2;
    pthread_t threads[num_threads];
    sem_init(&semaphore1, 0, 1);
    sem_init(&semaphore2, 0, 0);
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, (void *)increment, (void *)(intptr_t)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    return 0;
}
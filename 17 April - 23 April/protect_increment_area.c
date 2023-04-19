#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

long long count = 0;
sem_t semaphore;

void increment(void *th_id) {
    long long aux;
    sem_wait(&semaphore);
    aux = count;
    aux++;
    usleep(100);
    count = aux;
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    pthread_t threads[N];
    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, (void *)increment, (void *)(intptr_t)i);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    printf("Final count: %lld\n", count);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

long long count = 0;
int num_threads;
sem_t *semaphores;

void increment(void *th_id) {
    int i = 0;
    int id = (int)(intptr_t)th_id;
    while (i < 100) {
        sem_wait(&semaphores[id]);
        long long aux;
        aux = count;
        aux++;
        printf("count=%lld from thread %d\n", count, id + 1);
        count = aux;
        i++;
        sem_post(&semaphores[(id + 1) % num_threads]);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    semaphores = malloc(num_threads * sizeof(sem_t));

    for (int i = 0; i < num_threads; i++) {
        sem_init(&semaphores[i], 0, i == 0 ? 1 : 0);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, (void *)increment, (void *)(intptr_t)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        sem_destroy(&semaphores[i]);
    }
    free(semaphores);

    return 0;
}

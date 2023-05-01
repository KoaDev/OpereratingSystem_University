#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_H;
sem_t sem_S;
sem_t sem_O;

void* atomH(void* arg) {
    int id = *((int*) arg);
    printf("H atom %d arrived\n", id);
    sem_post(&sem_H);
    sem_wait(&sem_O);
    printf("H atom %d formed a molecule\n", id);
    return NULL;
}

void* atomS(void* arg) {
    int id = *((int*) arg);
    printf("S atom %d arrived\n", id);
    sem_post(&sem_S);
    sem_wait(&sem_O);
    printf("S atom %d formed a molecule\n", id);
    return NULL;
}

void* atomO(void* arg) {
    int id = *((int*) arg);
    printf("O atom %d arrived\n", id);
    sem_wait(&sem_H);
    sem_wait(&sem_S);
    sem_wait(&sem_H);
    printf("O atom %d formed a molecule\n", id);
    sem_post(&sem_O);
    sem_post(&sem_O);
    sem_post(&sem_O);
    sem_post(&sem_O);
    return NULL;
}

int main() {
    const int num_threads = 10;
    pthread_t threads[num_threads];
    int ids[num_threads];

    sem_init(&sem_H, 0, 0);
    sem_init(&sem_S, 0, 0);
    sem_init(&sem_O, 0, 0);

    for (int i = 0; i < num_threads; i++) {
        ids[i] = i;
        if (i % 5 == 0 || i % 5 == 1) {
            pthread_create(&threads[i], NULL, atomH, &ids[i]);
        } else if (i % 5 == 2) {
            pthread_create(&threads[i], NULL, atomS, &ids[i]);
        } else {
            pthread_create(&threads[i], NULL, atomO, &ids[i]);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_H);
    sem_destroy(&sem_S);
    sem_destroy(&sem_O);

    return 0;
}

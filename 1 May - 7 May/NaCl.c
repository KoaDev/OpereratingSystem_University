#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_Na;
sem_t sem_Cl;

void* atomNa(void* arg) {
    int id = *((int*) arg);
    printf("Na atom %d arrived\n", id);
    sem_post(&sem_Na);
    sem_wait(&sem_Cl);
    printf("Na atom %d formed a molecule\n", id);
    return NULL;
}

void* atomCl(void* arg) {
    int id = *((int*) arg);
    printf("Cl atom %d arrived\n", id);
    sem_post(&sem_Cl);
    sem_wait(&sem_Na);
    printf("Cl atom %d formed a molecule\n", id);
    return NULL;
}

int main() {
    const int num_threads = 10;
    pthread_t threads[num_threads];
    int ids[num_threads];

    sem_init(&sem_Na, 0, 0);
    sem_init(&sem_Cl, 0, 0);

    for (int i = 0; i < num_threads; i++) {
        ids[i] = i;
        if (i % 2 == 0) {
            pthread_create(&threads[i], NULL, atomNa, &ids[i]);
        } else {
            pthread_create(&threads[i], NULL, atomCl, &ids[i]);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_Na);
    sem_destroy(&sem_Cl);

    return 0;
}

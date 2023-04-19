#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_Na, sem_Cl, sem_H, sem_O, sem_S;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int NaCl_count = 0, H2O_count = 0, H2SO4_count = 0;

void atomNa(void) {
    sem_wait(&sem_Na);
    sem_wait(&sem_Cl);
    pthread_mutex_lock(&mutex);
    NaCl_count++;
    printf("Created NaCl molecule. Total NaCl molecules: %d\n", NaCl_count);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_Cl);
}

void atomCl(void) {
    sem_wait(&sem_Cl);
    sem_wait(&sem_Na);
    pthread_mutex_lock(&mutex);
    NaCl_count++;
    printf("Created NaCl molecule. Total NaCl molecules: %d\n", NaCl_count);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_Na);
}

void atomH(void) {
    sem_wait(&sem_H);
    sem_wait(&sem_H);
    sem_wait(&sem_O);
    pthread_mutex_lock(&mutex);
    H2O_count++;
    printf("Created H2O molecule. Total H2O molecules: %d\n", H2O_count);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_O);
}

void atomO(void) {
    sem_wait(&sem_O);
    sem_wait(&sem_H);
    sem_wait(&sem_H);
    pthread_mutex_lock(&mutex);
    H2O_count++;
    printf("Created H2O molecule. Total H2O molecules: %d\n", H2O_count);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_H);
    sem_post(&sem_H);
}

void atomS(void) {
    sem_wait(&sem_S);
    sem_wait(&sem_O);
    sem_wait(&sem_O);
    sem_wait(&sem_O);
    sem_wait(&sem_O);
    sem_wait(&sem_H);
    sem_wait(&sem_H);
    pthread_mutex_lock(&mutex);
    H2SO4_count++;
    printf("Created H2SO4 molecule. Total H2SO4 molecules: %d\n", H2SO4_count);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_H);
    sem_post(&sem_H);
    sem_post(&sem_O);
    sem_post(&sem_O);
    sem_post(&sem_O);
    sem_post(&sem_O);
}

void *atom_thread(void *arg) {
    void (*atom_func)(void) = arg;
    atom_func();
    return NULL;
}

int main() {
    srand(time(NULL));
    int num_threads = 20;
    pthread_t threads[num_threads];
    void (*atom_funcs[])(void) = {atomNa, atomCl, atomH, atomO, atomS};

    sem_init(&sem_Na, 0, 0);
    sem_init(&sem_Cl, 0, 0);
    sem_init(&sem_H, 0, 0);
    sem_init(&sem_O, 0, 0);
    sem_init(&sem_S, 0, 0);

    for (int i = 0; i < num_threads; i++) {
        int atom_idx = rand() % 5;
        void (*atom_func)(void) = atom_funcs[atom_idx];
        pthread_create(&threads[i], NULL, atom_thread, (void *)atom_func);
        sem_post(&sem_Na);
        sem_post(&sem_Cl);
        sem_post(&sem_H);
        sem_post(&sem_O);
        sem_post(&sem_S);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_Na);
    sem_destroy(&sem_Cl);
    sem_destroy(&sem_H);
    sem_destroy(&sem_O);
    sem_destroy(&sem_S);

    printf("Final molecule counts:\n");
    printf("NaCl: %d\n", NaCl_count);
    printf("H2O: %d\n", H2O_count);
    printf("H2SO4: %d\n", H2SO4_count);

    return 0;
}


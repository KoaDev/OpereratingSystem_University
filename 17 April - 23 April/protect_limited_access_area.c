#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int th_no = 0;
sem_t semaphore;

void limited_area(void *th_id) {
    sem_wait(&semaphore);
    th_no++;
    usleep(100);
    printf("The no of threads in the limited area is: %d\n", th_no);
    th_no--;
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <N> <M>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    pthread_t threads[N];
    sem_init(&semaphore, 0, M);

    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, (void *)limited_area, (void *)(intptr_t)i);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);

    return 0;
}

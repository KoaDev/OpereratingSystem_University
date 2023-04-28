#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long count = 0;
int M, N;
int current_thread = 0;
pthread_mutex_t lock;
pthread_cond_t cond;

void increment(void* arg)
{
    long long aux;
    int th_id = *((int*) arg);

    for(int i=1; i<=M; i++) {
        pthread_mutex_lock(&lock);
        while(th_id != current_thread) {
            pthread_cond_wait(&cond, &lock);
        }
        aux = count;
        printf("Thread %d read count having value %lld\n", th_id, aux);
        aux++;
        usleep(100);
        printf("Thread %d will update count with value %lld\n", th_id, aux);
        count = aux;
        current_thread = (current_thread + 1) % N;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 3) {
        printf("Usage: %s <number of threads> <number of increments per thread>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);

    pthread_t threads[N];
    int ids[N];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    for(int i=0; i<N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, (void*) increment, &ids[i]);
    }

    for(int i=0; i<N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final count value: %lld\n", count);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

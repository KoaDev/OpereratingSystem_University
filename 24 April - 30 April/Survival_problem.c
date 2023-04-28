#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CAPACITY 3
#define MISSIONARY 0
#define CANNIBAL 1

int N, boat_capacity = 0, missionary_count = 0, cannibal_count = 0;
pthread_mutex_t lock;
pthread_cond_t cond;

void* person(void* arg)
{
    int id = *((int*) arg);
    int role = rand() % 2;
    printf("%s %d arrived at the shore.\n", (role == MISSIONARY) ? "Missionary" : "Cannibal", id);
    pthread_mutex_lock(&lock);
    while(boat_capacity == MAX_CAPACITY || (role == MISSIONARY && cannibal_count > missionary_count) || boat_capacity > 0 && role != boat_capacity % 2) {
        pthread_cond_wait(&cond, &lock);
    }
    if(role == MISSIONARY) {
        missionary_count++;
    } else {
        cannibal_count++;
    }
    boat_capacity++;
    printf("%s %d boarded the boat.\n", (role == MISSIONARY) ? "Missionary" : "Cannibal", id);
    if(boat_capacity == MAX_CAPACITY || (N == missionary_count && cannibal_count == 0) || (missionary_count >= cannibal_count && missionary_count + cannibal_count == MAX_CAPACITY)) {
        printf("Boat is full. Crossing the river...\n");
        boat_capacity = 0;
        if(missionary_count < cannibal_count) {
            printf("Cannibals ate the missionaries. Game over.\n");
            exit(1);
        }
        missionary_count = 0;
        cannibal_count = 0;
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&lock);
    printf("%s %d left the shore.\n", (role == MISSIONARY) ? "Missionary" : "Cannibal", id);
    return NULL;
}

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("Usage: %s <number of people>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    pthread_t threads[N];
    int ids[N];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    srand(time(NULL));

    for(int i=0; i<N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, person, &ids[i]);
    }

    for(int i=0; i<N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    printf("All people crossed the river safely. Game over.\n");

    return 0;
}

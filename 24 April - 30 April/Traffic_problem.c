#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define GREEN 1
#define RED 0

int N, car_count = 0, intersection_state = RED;
pthread_mutex_t lock;
pthread_cond_t cond;

void* car(void* arg)
{
    int id = *((int*) arg);
    int street = rand() % 2;
    printf("Car %d arrived at street %d.\n", id, street);
    pthread_mutex_lock(&lock);
    while(intersection_state == RED || (intersection_state == GREEN && street != car_count % 2)) {
        pthread_cond_wait(&cond, &lock);
    }
    car_count++;
    printf("Car %d crossed the intersection.\n", id);
    if(car_count % 2 == 0) {
        intersection_state = (intersection_state == GREEN) ? RED : GREEN;
        printf("Traffic light changed to %s.\n", (intersection_state == GREEN) ? "GREEN" : "RED");
        car_count = 0;
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&lock);
    printf("Car %d left the intersection.\n", id);
    return NULL;
}

void* traffic_lights(void* arg)
{
    while(1) {
        pthread_mutex_lock(&lock);
        intersection_state = (intersection_state == GREEN) ? RED : GREEN;
        printf("Traffic light changed to %s.\n", (intersection_state == GREEN) ? "GREEN" : "RED");
        car_count = 0;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
        sleep(5); // change traffic lights every 5 seconds
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("Usage: %s <number of cars>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    pthread_t threads[N+1];
    int ids[N];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    srand(time(NULL));

    pthread_create(&threads[N], NULL, traffic_lights, NULL);

    for(int i=0; i<N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, car, &ids[i]);
    }

    for(int i=0; i<N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_join(threads[N], NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 5

struct car_args {
    int car_id;
    int car_direction;
};

int passing_direction = 0;
int cars_on_bridge = 0;
int cars_waiting[2] = {0, 0};

pthread_mutex_t bridge_mutex;
pthread_cond_t direction_changed;
pthread_cond_t bridge_empty;

void enter_bridge(int car_id, int car_direction) {
    pthread_mutex_lock(&bridge_mutex);

    cars_waiting[car_direction]++;

    while (car_direction != passing_direction || cars_on_bridge == MAX) {
        pthread_cond_wait(&direction_changed, &bridge_mutex);
    }

    cars_waiting[car_direction]--;
    cars_on_bridge++;

    pthread_mutex_unlock(&bridge_mutex);
}

void pass_bridge(int car_id, int car_direction) {
    printf("Car %d is passing the bridge in direction %d\n", car_id, car_direction);
    sleep(1); // Simulate the time the car takes to cross the bridge
}

void exit_bridge(int car_id, int car_direction) {
    pthread_mutex_lock(&bridge_mutex);

    cars_on_bridge--;

    if (cars_on_bridge == 0 && cars_waiting[1 - car_direction] > 0) {
        pthread_cond_broadcast(&bridge_empty);
    }

    pthread_mutex_unlock(&bridge_mutex);
}

void* car(void* arg)
{
    struct car_args* car_info = (struct car_args*) arg;

    enter_bridge(car_info->car_id, car_info->car_direction);
    pass_bridge(car_info->car_id, car_info->car_direction);
    exit_bridge(car_info->car_id, car_info->car_direction);

    return NULL;
}

void change_passing_direction() {
    pthread_mutex_lock(&bridge_mutex);

    passing_direction = 1 - passing_direction;
    pthread_cond_broadcast(&direction_changed);

    while (cars_on_bridge > 0) {
        pthread_cond_wait(&bridge_empty, &bridge_mutex);
    }

    pthread_mutex_unlock(&bridge_mutex);
}

void* traffic_controller(void* arg)
{
    while (1) {
        sleep(5);
        change_passing_direction();
    }
}

int main()
{
    const int num_cars = 10;
    pthread_t car_threads[num_cars];
    struct car_args car_info[num_cars];

    pthread_t traffic_controller_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&bridge_mutex, NULL);
    pthread_cond_init(&direction_changed, NULL);
    pthread_cond_init(&bridge_empty, NULL);

    // Create and run traffic_controller thread
    if (pthread_create(&traffic_controller_thread, NULL, traffic_controller, NULL) != 0) {
        perror("Cannot create traffic_controller thread");
        exit(1);
    }

    // Create and run car threads
    for (int i = 0; i < num_cars; i++) {
        car_info[i].car_id = i;
        car_info[i].car_direction = i % 2;

        if (pthread_create(&car_threads[i], NULL, car, &car_info[i]) != 0) {
            perror("Cannot create car threads");
            exit(2);
        }
    }

    // Join car threads
    for (int i = 0; i < num_cars; i++) {
        if (pthread_join(car_threads[i], NULL) != 0) {
            perror("Cannot join car threads");
            exit(3);
        }
    }

    // Cancel traffic_controller thread
    pthread_cancel(traffic_controller_thread);

    // Join traffic_controller thread
    if (pthread_join(traffic_controller_thread, NULL) != 0) {
        perror("Cannot join traffic_controller thread");
        exit(4);
    }

    // Clean up mutex and condition variables
    pthread_mutex_destroy(&bridge_mutex);
    pthread_cond_destroy(&direction_changed);
    pthread_cond_destroy(&bridge_empty);

    return 0;
}


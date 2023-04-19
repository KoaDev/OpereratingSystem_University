#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

const char *semaphore1_name = "/semaphore1";
const char *semaphore2_name = "/semaphore2";

void car_movement(int street, sem_t *semaphore) {
    while (1) {
        sem_wait(semaphore);
        printf("Car passing through street %d\n", street);
        sleep(1);
        sem_post(semaphore);
        sleep(rand() % 3 + 1);
    }
}

void control_traffic_lights(sem_t *semaphore1, sem_t *semaphore2) {
    while (1) {
        sleep(rand() % 3 + 2);
        sem_wait(semaphore1);
        sem_wait(semaphore2);
        printf("Changing traffic lights\n");
        sem_post(semaphore1);
        sem_post(semaphore2);
    }
}

int main() {
    srand(time(NULL));

    sem_t *semaphore1 = sem_open(semaphore1_name, O_CREAT, 0600, 1);
    sem_t *semaphore2 = sem_open(semaphore2_name, O_CREAT, 0600, 0);

    pid_t street1_pid = fork();
    if (street1_pid == 0) {
        car_movement(1, semaphore1);
        exit(0);
    }

    pid_t street2_pid = fork();
    if (street2_pid == 0) {
        car_movement(2, semaphore2);
        exit(0);
    }

    control_traffic_lights(semaphore1, semaphore2);

    // Cleanup
    kill(street1_pid, SIGKILL);
    kill(street2_pid, SIGKILL);

    sem_unlink(semaphore1_name);
    sem_unlink(semaphore2_name);
    sem_close(semaphore1);
    sem_close(semaphore2);

    return 0;
}

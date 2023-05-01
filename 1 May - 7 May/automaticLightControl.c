#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int no_of_persons;
int no_of_persons_left;
int no_of_persons_inside = 0;

sem_t person_arrived;
sem_t light_on;
sem_t room_empty;

void* person(void* arg)
{
    int id = *(int*) arg;
    printf("Person %d has just arrived in front of the room's door\n", id);

    sem_post(&person_arrived); // Signal the light controller that a person has arrived
    sem_wait(&light_on);       // Wait for the light to be on before entering the room

    no_of_persons_inside++;
    printf("Person %d is in the room's door. No of persons in room = %d\n", id, no_of_persons_inside);

    usleep(rand() % 1000); // Simulate the time the person spends in the room

    no_of_persons_inside--;
    printf("Person %d is leaving the room's door. No of persons in room = %d\n", id, no_of_persons_inside);

    if (no_of_persons_inside == 0) {
        sem_post(&room_empty); // Signal the controller that the room is empty
    }

    no_of_persons_left--;
}

void* light_controller(void* arg)
{
    while (no_of_persons_left > 0) {
        sem_wait(&person_arrived); // Wait for a person to arrive

        printf("Light is ON\n");
        sem_post(&light_on);       // Allow the person(s) to enter the room

        sem_wait(&room_empty);     // Wait for the room to become empty (last person to leave)
        printf("Light is OFF\n");
    }
}

int main(int argc, char **argv)
{
    pthread_t *person_th;
    int       *person_arg;
    pthread_t controller_th;

    if (argc != 2) {
        printf("Error. Usage: %s <no_of_persons>\n", argv[0]);
        exit(1);
    }

    sscanf(argv[1], "%d", &no_of_persons);
    no_of_persons_left = no_of_persons;

    sem_init(&person_arrived, 0, 0);
    sem_init(&light_on, 0, 0);
    sem_init(&room_empty, 0, 0);

    person_th = (pthread_t*) malloc(no_of_persons * sizeof(pthread_t));
    if (person_th == NULL) {
        perror("Cannot allocate memory for threads");
        exit(2);
    }

    person_arg = (int*) malloc(no_of_persons * sizeof(int));
    if (person_arg == NULL) {
        perror("Cannot allocate memory for person ids");
        exit(2);
    }

    if (pthread_create(&controller_th, NULL, light_controller, NULL) != 0) {
        perror("Cannot create controller threads");
        exit(3);
    }

    for (int i = 0; i < no_of_persons; i++) {
        person_arg[i] = i;
        if (pthread_create(&person_th[i], NULL, person, &person_arg[i]) != 0) {
            perror("Cannot create person threads");
            exit(3);
        }
    }

    for (int i = 0; i < no_of_persons; i++) {
        if (pthread_join(person_th[i], NULL) != 0) {
            perror("Cannot join person threads");
            exit(4);
        }
    }
    if (pthread_join(controller_th, NULL) != 0) {
        perror("Cannot join controller thread");
        exit(4);
    }

// Clean up semaphores
    sem_destroy(&person_arrived);
    sem_destroy(&light_on);
    sem_destroy(&room_empty);

// Free allocated memory
    free(person_th);
    free(person_arg);

    return 0;
}
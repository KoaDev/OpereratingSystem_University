#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void sigusr1_handler(int signum) {
    printf("SIGUSR1 signal received by main thread.\n");
}

void* worker_thread_func(void* arg) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    sleep(5);
    printf("Worker thread finished.\n");

    return NULL;
}

int main() {
    signal(SIGUSR1, sigusr1_handler);

    pthread_t worker_thread;
    pthread_create(&worker_thread, NULL, worker_thread_func, NULL);

    printf("Sending SIGUSR1 to the process.\n");
    kill(getpid(), SIGUSR1);

    pthread_join(worker_thread, NULL);
    printf("Main thread finished.\n");

    return 0;
}

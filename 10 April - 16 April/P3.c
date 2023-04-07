#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int start;
    int end;
    unsigned long long* result;
} Range;

void* calculate_product(void* arg) {
    Range* range = (Range*) arg;
    unsigned long long product = 1;

    for (int i = range->start; i <= range->end; ++i) {
        product *= i;
    }

    *range->result = product;

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <M>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int M = atoi(argv[2]);

    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    Range* ranges = malloc(sizeof(Range) * num_threads);
    unsigned long long* results = malloc(sizeof(unsigned long long) * num_threads);

    int chunk_size = M / num_threads;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Create N threads
    for (int i = 0; i < num_threads; ++i) {
        ranges[i].start = i * chunk_size + 1;
        ranges[i].end = (i == num_threads - 1) ? M : (i + 1) * chunk_size;
        ranges[i].result = &results[i];
        pthread_create(&threads[i], NULL, calculate_product, &ranges[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Combine the results of each thread
    unsigned long long total_product = 1;
    for (int i = 0; i < num_threads; ++i) {
        total_product *= results[i];
    }

    gettimeofday(&end, NULL);

    printf("Product: %llu\n", total_product);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    printf("Time elapsed: %.6f seconds\n", elapsed);

    free(threads);
    free(ranges);
    free(results);

    return 0;
}

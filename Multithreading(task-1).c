#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared data
int* numbers;   // array of numbers
int count;      // number of numbers

// Worker thread functions

void* average_thread(void* arg) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    double* avg = malloc(sizeof(double));
    *avg = sum / count;
    pthread_exit(avg);  // return the result
}

void* max_thread(void* arg) {
    int max = numbers[0];
    for (int i = 1; i < count; i++) {
        if (numbers[i] > max)
            max = numbers[i];
    }
    int* result = malloc(sizeof(int));
    *result = max;
    pthread_exit(result);
}

void* min_thread(void* arg) {
    int min = numbers[0];
    for (int i = 1; i < count; i++) {
        if (numbers[i] < min)
            min = numbers[i];
    }
    int* result = malloc(sizeof(int));
    *result = min;
    pthread_exit(result);
}

// Main function
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s num1 num2 num3 ...\n", argv[0]);
        return 1;
    }

    count = argc - 1;
    numbers = malloc(count * sizeof(int));

    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }

    pthread_t tid_avg, tid_max, tid_min;
    void* avg_result;
    void* max_result;
    void* min_result;

    // Create threads
    pthread_create(&tid_avg, NULL, average_thread, NULL);
    pthread_create(&tid_max, NULL, max_thread, NULL);
    pthread_create(&tid_min, NULL, min_thread, NULL);

    // Wait for threads and collect results
    pthread_join(tid_avg, &avg_result);
    pthread_join(tid_max, &max_result);
    pthread_join(tid_min, &min_result);

    // Print results
    printf("Average: %.2f\n", *(double*)avg_result);
    printf("Maximum: %d\n", *(int*)max_result);
    printf("Minimum: %d\n", *(int*)min_result);

    // Free dynamically allocated memory
    free(avg_result);
    free(max_result);
    free(min_result);
    free(numbers);

    return 0;
}

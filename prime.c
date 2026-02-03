#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int* list;
int size;

void* prime(void* arg)
{
    int primeCount = 0;

    for (int i = 0; i < size; i++)
    {
        int count = 0;

        if (list[i] <= 1)
            continue;

        for (int j = 2; j < list[i]; j++)
        {
            if (list[i] % j == 0)
            {
                count++;
                break;   // no need to continue
            }
        }

        if (count == 0)
            primeCount++;
    }

    int* result = malloc(sizeof(int));
    *result = primeCount;
    return result;
}

int main(int argc, char* argv[])
{
    size = argc - 1;
    list = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        list[i] = atoi(argv[i + 1]);

    pthread_t t;
    pthread_create(&t, NULL, prime, NULL);

    void* res;
    pthread_join(t, &res);

    int* primes = (int*)res;
    printf("Total prime numbers = %d\n", *primes);

    free(primes);
    free(list);

    return 0;
}

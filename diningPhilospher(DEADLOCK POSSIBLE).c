#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define forkSize 5

sem_t eatLock[forkSize];

void* eating(void* arg)
{
    int index = *(int*)arg;
    int left = index;
    int right = (left + 1) % forkSize;

    printf("Philosopher %d is thinking\n", index);

    sem_wait(&eatLock[right]);
    sem_wait(&eatLock[left]);

    printf("Philosopher %d is eating\n", index);
    sleep(1);

    sem_post(&eatLock[left]);
    sem_post(&eatLock[right]);

    return NULL;
}

int main()
{
    pthread_t tid[forkSize];
    int id[forkSize];

    for(int i = 0; i < forkSize; i++) {
        sem_init(&eatLock[i], 0, 1);
        id[i] = i;
    }

    for(int i = 0; i < forkSize; i++) {
        pthread_create(&tid[i], NULL, eating, &id[i]);
    }

    for(int i = 0; i < forkSize; i++) {
        pthread_join(tid[i], NULL);
    }

    for(int i = 0; i < forkSize; i++) {
        sem_destroy(&eatLock[i]);
    }

    return 0;
}

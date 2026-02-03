#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int readCount = 0;
int sharedData = 0;

pthread_mutex_t mutex;
sem_t wrtlock;

void* writer(void* arg)
{
    sem_wait(&wrtlock);

    sharedData++;
    printf("Writer is writing data = %d\n", sharedData);

    sem_post(&wrtlock);
    return NULL;
}

void* reader(void* arg)
{
    pthread_mutex_lock(&mutex);
    readCount++;

    if (readCount == 1)
    {
        sem_wait(&wrtlock);   // first reader blocks writer(READER PRIORITY)
    }
    pthread_mutex_unlock(&mutex);

    // Reading section
    printf("Reader is reading data = %d\n", sharedData);

    // Leaving section
    pthread_mutex_lock(&mutex);
    readCount--;

    if (readCount == 0)
    {
        sem_post(&wrtlock);   // last reader allows writer(READER PRIORITY)
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t w, r1, r2, r3;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrtlock, 0, 1);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&r3, NULL, reader, NULL);
    pthread_create(&w, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w, NULL);

    sem_destroy(&wrtlock);
    pthread_mutex_destroy(&mutex);

    return 0;
}
NOtes:

PROBLEM with your earlier solution
⚠️ Writer Starvation

Imagine this timeline:

Reader 1 enters
Reader 2 enters
Writer arrives → waits
Reader 3 arrives → allowed
Reader 4 arrives → allowed
Reader 5 arrives → allowed
...


❌ Writer may wait forever

➡️ This is EXACTLY what your lab manual says:

"Give readers priority → writers may starve"
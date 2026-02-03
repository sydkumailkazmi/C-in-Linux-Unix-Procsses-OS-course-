#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_READERS 5

int readCount = 0;
int sharedData = 0;

pthread_mutex_t mutex;

sem_t wrtlock;        // writer exclusive access
sem_t altLock;        // alternation control
sem_t readerLimit;   // max 5 readers

void* reader(void* arg)
{
    // wait for reader turn
    sem_wait(&altLock);

    // wait if 5 readers already inside
    sem_wait(&readerLimit);

    pthread_mutex_lock(&mutex);
    readCount++;

    // first reader blocks writers
    if (readCount == 1)
    {
        sem_wait(&wrtlock);
    }

    pthread_mutex_unlock(&mutex);

    // allow next reader
    sem_post(&altLock);

    // reading section
    printf("Reader reading data = %d\n", sharedData);
    sleep(1);

    // leaving section
    pthread_mutex_lock(&mutex);
    readCount--;

    // last reader gives chance to writer
    if (readCount == 0)
    {
        sem_post(&wrtlock);
    }

    pthread_mutex_unlock(&mutex);

    sem_post(&readerLimit);

    return NULL;
}

void* writer(void* arg)
{
    // wait for writer turn
    sem_wait(&altLock);

    sem_wait(&wrtlock);

    // writing section
    sharedData++;
    printf("Writer writing data = %d\n", sharedData);
    sleep(1);

    sem_post(&wrtlock);

    // give turn back to readers
    sem_post(&altLock);

    return NULL;
}

int main()
{
    pthread_t r[10], w[3];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrtlock, 0, 1);
    sem_init(&altLock, 0, 1);
    sem_init(&readerLimit, 0, MAX_READERS);

    for (int i = 0; i < 10; i++)
        pthread_create(&r[i], NULL, reader, NULL);

    for (int i = 0; i < 3; i++)
        pthread_create(&w[i], NULL, writer, NULL);

    for (int i = 0; i < 10; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(w[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrtlock);
    sem_destroy(&altLock);
    sem_destroy(&readerLimit);

    return 0;
}

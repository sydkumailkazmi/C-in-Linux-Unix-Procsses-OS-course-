#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int readCount = 0;
int sharedData = 0;

pthread_mutex_t mutex;     // protects readCount
sem_t wrtlock;             // exclusive access to shared data
sem_t orderMutex;          // preserves arrival order

void* writer(void* arg)
{
    // wait for turn (arrival order)
    sem_wait(&orderMutex);

    // exclusive access to shared data
    sem_wait(&wrtlock);

    // allow next thread to line up
    sem_post(&orderMutex);

    // writing section
    sharedData++;
    printf("Writer writing data = %d\n", sharedData);
    sleep(1);

    // release shared data
    sem_post(&wrtlock);

    return NULL;
}

void* reader(void* arg)
{
    // wait for turn (arrival order)
    sem_wait(&orderMutex);

    pthread_mutex_lock(&mutex);
    readCount++;

    // first reader blocks writers
    if (readCount == 1)
    {
        sem_wait(&wrtlock);
    }

    // allow next thread to line up
    sem_post(&orderMutex);

    pthread_mutex_unlock(&mutex);

    // reading section
    printf("Reader reading data = %d\n", sharedData);
    sleep(1);

    // leaving section
    pthread_mutex_lock(&mutex);
    readCount--;

    // last reader allows writer
    if (readCount == 0)
    {
        sem_post(&wrtlock);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t r1, r2, r3, w1, w2;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrtlock, 0, 1);
    sem_init(&orderMutex, 0, 1); //basically aik drwaza jis pr aik hi time pr aik hi semaphore execute hoga order mein 

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&r3, NULL, reader, NULL);
    pthread_create(&w2, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    sem_destroy(&wrtlock);
    sem_destroy(&orderMutex);
    pthread_mutex_destroy(&mutex);

    return 0;
}

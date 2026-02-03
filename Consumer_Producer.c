#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int p = 0;
int c = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void* producer(void* arg)
{
    int item = 0;              // produced item

  for (int i = 0; i < 5; i++)
    {
		item=i+1;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[p] = item;
    printf("Item produced: %d\n", item);
    p = (p + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
	}
    return NULL;
}

void* consumer(void* arg)
{
    int item;
  for (int i = 0; i < 5; i++)
    {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    item = buffer[c];
    printf("Item consumed: %d\n", item);
    c = (c + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
	}
    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);  //instead of using loop use loop here to call one producer or one consumer one by one BUT THERE WILL BE NO COCURRENCY AS BUFFER WILL NEVER BE FULL

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);

    return 0;
}

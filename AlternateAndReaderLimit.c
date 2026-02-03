#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t mutex;
sem_t readersem;
sem_t writersem;

int totalreaders = 8;
int totalwriters = 3;

int activeReaders = 0;
int readersBatchCount = 0;
int readersLeft = 8;
int writersLeft = 3;

void* reader(void* arg) {
    int id = *(int*)arg;

    sem_wait(&readersem);   // wait for reader turn

    sem_wait(&mutex);
    activeReaders++;
    readersBatchCount++;
    readersLeft--;
    sem_post(&mutex);

    printf("Reader %d is reading\n", id);
    sleep(1);
    printf("Reader %d finished reading\n", id);

    sem_wait(&mutex);
    activeReaders--;

    // If batch done OR no readers left AND last active reader
    if ((readersBatchCount == 5 || readersLeft == 0) && activeReaders == 0) {
        readersBatchCount = 0;
        sem_post(&writersem);   // allow ONE writer
    }
    else {
        sem_post(&readersem);   // allow next reader in batch
    }
	
	

    sem_post(&mutex);
    pthread_exit(NULL);
}

void* writer(void* arg) {
    int id = *(int*)arg;

    sem_wait(&writersem);   // wait for writer turn

    sem_wait(&mutex);
    writersLeft--;
    sem_post(&mutex);

    printf("Writer %d is writing\n", id);
    sleep(1);
    printf("Writer %d finished writing\n", id);

    sem_wait(&mutex);
    if (readersLeft > 0) {
        sem_post(&readersem);   // start next reader batch
    }
    else if (writersLeft > 0) {
        sem_post(&writersem);   // next writer
    }
    sem_post(&mutex);

    pthread_exit(NULL);
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&readersem, 0, 1);   // readers start first
    sem_init(&writersem, 0, 0);   // writers blocked initially

    pthread_t r[8], w[3];
    int r_id[8], w_id[3];

    for (int i = 0; i < 8; i++) {
        r_id[i] = i;
        pthread_create(&r[i], NULL, reader, &r_id[i]);
    }

    for (int i = 0; i < 3; i++) {
        w_id[i] = i;
        pthread_create(&w[i], NULL, writer, &w_id[i]);
    }

    for (int i = 0; i < 8; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(w[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&readersem);
    sem_destroy(&writersem);

    return 0;
}
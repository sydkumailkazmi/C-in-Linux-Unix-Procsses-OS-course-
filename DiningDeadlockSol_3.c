//agr even hay id philospher ka toh pehlay right uthaye phr left vice versa

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

#define forkSize 5

sem_t chopsticks[forkSize];

void* philospher(void* arg)
{
    int* id = (int*)arg;
    int left = *id;
    int right = (left + 1) % forkSize;

    if(left % 2 == 0)
    {
        sem_wait(&chopsticks[right]);
        sem_wait(&chopsticks[left]);    
    }
    else{
        sem_wait(&chopsticks[left]);    
        sem_wait(&chopsticks[right]);
    }

    printf("Philospher- %d eating \n", left);
    sleep(1);

    sem_post(&chopsticks[right]);
    sem_post(&chopsticks[left]);

    return NULL;
}

int main()
{
    int id[forkSize]; //philospher ka id
    pthread_t tid[forkSize];

    for(int i=0; i<forkSize; i++)
    {
        sem_init(&chopsticks[i],0,1);
    }

    for(int i=0; i<forkSize; i++)
    {
        id[i] = i;
        pthread_create(&tid[i], NULL, philospher, &id[i]);
    }

    for(int i=0; i<forkSize; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for(int i=0; i<forkSize; i++)
    {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
Breaking the Circular Wait:

In a normal scenario, all philosophers pick left first, creating: 0→1→2→3→4→0 (circle!)
With asymmetry, at least ONE philosopher (e.g., Phil 4) picks in reverse order
This breaks the circular dependency chain
No cycle = No deadlock!

How It Works:
The asymmetric ordering ensures that at least one adjacent pair of philosophers compete for the same chopstick first, preventing the circular wait condition necessary for deadlock.
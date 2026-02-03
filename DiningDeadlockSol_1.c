//only 4 philosphers allowed if 5 chopsticks

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define forkSize 5

sem_t chopsticks[forkSize]; //binary array
sem_t room; //counting to keep the philospher in check

void* philospher(void* arg)
{
    int* id = (int*) arg;
    int left = *id;
    int right = (left + 1) % forkSize;

    sem_wait(&room); //making sure max 4 hi ayein

    sem_wait(&chopsticks[left]);
    sem_wait(&chopsticks[right]);

    printf("Philospher- %d eating. \n", left);

    sem_post(&chopsticks[left]);
    sem_post(&chopsticks[right]);

    sem_post(&room);

    return NULL;
}

int main()
{
    int id[forkSize];
    pthread_t tid[forkSize];

    sem_init(&room, 0, forkSize - 1); //5th wala wait kreyga 

    for (int i = 0; i < forkSize; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
        id[i] = i;
    }

    for (int i = 0; i < forkSize; i++)
    {
        pthread_create(&tid[i], NULL, philospher, &id[i]);
    }

    for (int i = 0; i < forkSize; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < forkSize; i++)
    {
        sem_destroy(&chopsticks[i]);
    }

    sem_destroy(&room);

    return 0;
}
# Why This Prevents Deadlock:

**The Key Insight**: With 5 chopsticks but only 4 philosophers allowed at the table:
```
Scenario without room semaphore (DEADLOCK possible):
Phil 0: holds chopstick 0, waits for 1
Phil 1: holds chopstick 1, waits for 2
Phil 2: holds chopstick 2, waits for 3
Phil 3: holds chopstick 3, waits for 4
Phil 4: holds chopstick 4, waits for 0  ← CIRCULAR WAIT!

Scenario with room semaphore (NO DEADLOCK):
Phil 0: enters room, holds chopstick 0, waits for 1
Phil 1: enters room, holds chopstick 1, waits for 2
Phil 2: enters room, holds chopstick 2, waits for 3
Phil 3: enters room, holds chopstick 3, waits for 4
Phil 4: BLOCKED at room (can't enter) ← BREAKS THE CYCLE!

At least one of Phil 0-3 will get both chopsticks and finish!
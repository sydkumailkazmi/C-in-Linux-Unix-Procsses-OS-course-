#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

#define forkSize 5

sem_t chopsticks[forkSize];
pthread_mutex_t mutex;

void* philospher(void* arg)
{
    int* id = (int*) arg;
    int left = *id;
    int right = (left + 1) % forkSize;
    
    // CRITICAL SECTION - atomically pick up both chopsticks
    pthread_mutex_lock(&mutex);
    sem_wait(&chopsticks[left]);   // Fixed: Added & before chopsticks
    sem_wait(&chopsticks[right]);  // Fixed: Added & before chopsticks
    pthread_mutex_unlock(&mutex);
    
    printf("Philosopher-%d eating\n", left);
    sleep(1); // Simulate eating
    
    // Release both chopsticks (no mutex needed here)
    sem_post(&chopsticks[left]);   // Fixed: Added & before chopsticks
    sem_post(&chopsticks[right]);  // Fixed: Added & before chopsticks
    
    return NULL;
}

int main()
{
    int id[forkSize];
    pthread_t tid[forkSize];
    
    pthread_mutex_init(&mutex, NULL);
    
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
    
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

## Key Fixes:

1. **Critical Bug Fixed**: You forgot the `&` before `chopsticks[i]` in `sem_wait()` and `sem_post()` calls. Semaphore functions need a pointer to the semaphore, not the semaphore value itself.

2. **How This Prevents Deadlock**:
   - The mutex ensures that picking up BOTH chopsticks is atomic
   - A philosopher can only pick up chopsticks if BOTH are available
   - No partial pickup = no circular wait = no deadlock

3. **Why Releasing Doesn't Need Mutex**:
   - `sem_post()` is already atomic
   - Releasing resources doesn't create race conditions

## How It Works:
```
Philosopher 0: Lock mutex → Check both chopsticks 0,1 → Pick both → Unlock → Eat → Release
Philosopher 1: Try lock mutex (blocked if 0 still picking) → Eventually picks 1,2 → ...
void* reader(void* arg)
{
    // STEP 1: Get in line (take a ticket)
    sem_wait(&orderMutex);  // "I'm waiting for my turn"
    
    // STEP 2: Update the reader count
    pthread_mutex_lock(&mutex);
    readCount++;
    
    // STEP 3: First reader? Lock the room!
    if (readCount == 1)  // "I'm the first one here"
    {
        sem_wait(&wrtlock);  // Block writers from entering
    }
    
    // STEP 4: I got access! Let the next person in line get their turn
    sem_post(&orderMutex);  // "I've been served, next person please!"
    pthread_mutex_unlock(&mutex);
    
    // STEP 5: READ (Critical Section)
    printf("Reading data = %d\n", sharedData);
    sleep(1);
    
    // STEP 6: Done reading, update count
    pthread_mutex_lock(&mutex);
    readCount--;
    
    // STEP 7: Last reader? Unlock the room!
    if (readCount == 0)  // "I'm the last one leaving"
    {
        sem_post(&wrtlock);  // Allow writers to enter
    }
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}
```

### 🔍 Deep Dive - Reader Logic

**Why `orderMutex`?**
```
Without orderMutex:
Reader1 arrives → starts reading
Reader2 arrives → joins Reader1
Writer arrives → waits
Reader3 arrives → joins Reader1 & Reader2  ← Writer still waiting!
Reader4 arrives → joins others              ← Writer STARVES!

With orderMutex:
Reader1 arrives → takes orderMutex → starts reading → releases orderMutex
Reader2 arrives → takes orderMutex → joins Reader1 → releases orderMutex
Writer arrives → takes orderMutex → WAITS for readers to finish
Reader3 arrives → BLOCKED (Writer holds orderMutex position)
```

**The "Light Switch" Pattern:**
```
First reader entering:
  if (readCount == 1)      // "I'm first!"
    sem_wait(&wrtlock);    // "Turn the lights ON, lock the door"

Last reader leaving:
  if (readCount == 0)      // "I'm last!"
    sem_post(&wrtlock);    // "Turn the lights OFF, unlock the door"
	
	
	
	void* writer(void* arg)
{
    // STEP 1: Get in line (take a ticket)
    sem_wait(&orderMutex);  // "I'm waiting for my turn"
    
    // STEP 2: Request exclusive access
    sem_wait(&wrtlock);     // "Lock the entire room, kick everyone out"
    
    // STEP 3: I got access! Let the next person in line get their turn
    sem_post(&orderMutex);  // "I've been served, next person please!"
    
    // STEP 4: WRITE (Critical Section)
    sharedData++;
    printf("Writing data = %d\n", sharedData);
    sleep(1);
    
    // STEP 5: Done writing, release access
    sem_post(&wrtlock);     // "Unlock the room, others can enter"
    
    return NULL;
}
```
//orderMutex say queue ban jati agr write 2 reader kay baad aya hay toh iska mtlb yeh nhi kay aur reader ayein gay toh write ko unka bhi wait krna prna 
## 🎬 Complete Example Walkthrough

Let's trace: **R1 → R2 → W1 → R3 → W2**
```
Time | Thread | Action                          | orderMutex | wrtlock | readCount
-----|--------|----------------------------------|------------|---------|----------
  0  | R1     | sem_wait(&orderMutex)           | TAKEN      | 1       | 0
  1  | R1     | readCount++ (now 1)             | TAKEN      | 1       | 1
  2  | R1     | sem_wait(&wrtlock) [first!]     | TAKEN      | TAKEN   | 1
  3  | R1     | sem_post(&orderMutex)           | FREE       | TAKEN   | 1
  4  | R1     | "Reading data = 0"              | FREE       | TAKEN   | 1
-----|--------|----------------------------------|------------|---------|----------
  5  | R2     | sem_wait(&orderMutex)           | TAKEN      | TAKEN   | 1
  6  | R2     | readCount++ (now 2)             | TAKEN      | TAKEN   | 2
  7  | R2     | if(readCount==1)? NO, skip      | TAKEN      | TAKEN   | 2
  8  | R2     | sem_post(&orderMutex)           | FREE       | TAKEN   | 2
  9  | R2     | "Reading data = 0"              | FREE       | TAKEN   | 2
       ↑ R1 and R2 reading together! Multiple readers OK!
-----|--------|----------------------------------|------------|---------|----------
 10  | W1     | sem_wait(&orderMutex)           | TAKEN      | TAKEN   | 2
 11  | W1     | sem_wait(&wrtlock) [BLOCKED!]   | TAKEN      | TAKEN   | 2
       ↑ Writer WAITS because readers still have wrtlock
-----|--------|----------------------------------|------------|---------|----------
 12  | R3     | sem_wait(&orderMutex) [BLOCKED] | TAKEN      | TAKEN   | 2
       ↑ R3 can't get orderMutex (W1 has it), must wait in line!
-----|--------|----------------------------------|------------|---------|----------
 13  | R1     | Done reading, readCount-- (=1)  | TAKEN      | TAKEN   | 1
 14  | R1     | if(readCount==0)? NO            | TAKEN      | TAKEN   | 1
 15  | R1     | exits                           | TAKEN      | TAKEN   | 1
-----|--------|----------------------------------|------------|---------|----------
 16  | R2     | Done reading, readCount-- (=0)  | TAKEN      | TAKEN   | 0
 17  | R2     | if(readCount==0)? YES!          | TAKEN      | TAKEN   | 0
 18  | R2     | sem_post(&wrtlock) [last out!]  | TAKEN      | FREE    | 0
 19  | R2     | exits                           | TAKEN      | FREE    | 0
       ↑ Last reader releases wrtlock, now W1 can proceed!
-----|--------|----------------------------------|------------|---------|----------
 20  | W1     | sem_wait(&wrtlock) [SUCCESS!]   | TAKEN      | TAKEN   | 0
 21  | W1     | sem_post(&orderMutex)           | FREE       | TAKEN   | 0
 22  | W1     | "Writing data = 1"              | FREE       | TAKEN   | 0
 23  | W1     | sem_post(&wrtlock)              | FREE       | FREE    | 0
       ↑ Now R3 can take orderMutex!
-----|--------|----------------------------------|------------|---------|----------
 24  | R3     | sem_wait(&orderMutex)           | TAKEN      | FREE    | 0
 25  | R3     | readCount++ (now 1)             | TAKEN      | FREE    | 1
 26  | R3     | sem_wait(&wrtlock) [first!]     | TAKEN      | TAKEN   | 1
 27  | R3     | sem_post(&orderMutex)           | FREE       | TAKEN   | 1
 28  | R3     | "Reading data = 1"              | FREE       | TAKEN   | 1
 29  | R3     | readCount-- (=0), last out      | FREE       | TAKEN   | 0
 30  | R3     | sem_post(&wrtlock)              | FREE       | FREE    | 0
-----|--------|----------------------------------|------------|---------|----------
 31  | W2     | Same as W1...                   | ...        | ...     | ...
```

## 🔑 Key Insights

### 1. **orderMutex = FIFO Queue**
```
Arrival: R1 → R2 → W1 → R3 → W2
Service: R1,R2 together → W1 → R3 → W2
         ^^^ These got orderMutex consecutively
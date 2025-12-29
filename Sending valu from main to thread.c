#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* worker(void* arg){
    printf("This is thread() worker\n");
    int* num=(int*)arg;
    printf("The number we got from main() %d",*num);
    int mul=(*num)*10;
     printf("The number we got from main() *10 in thread %d",mul);

    return NULL;
}

int main(){
    printf("This is main()\n");

    pthread_t t1;

    int x=10;
    pthread_create(&t1, NULL, worker, &x); //we send in reference? which void can point as it is generic pointer
    pthread_join(t1, NULL);



    return 0;
}

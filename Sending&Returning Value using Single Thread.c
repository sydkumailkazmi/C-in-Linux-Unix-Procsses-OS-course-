#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* worker(void* arg){
    printf("This is thread() worker\n");
    int* num=(int*)arg; //always pointer to pointer conversion RMEMBER
    printf("The number we got from main() %d",*num);
    int double=(*num)*2;

    int* ret=malloc(sizeof(int)); //we need to dynnamically assign value so it keeps alive in the main() for use once this thread function dies alongside its variables
    *ret=double;

    return ret;
}

int main(){
    printf("This is main()\n");

    pthread_t t1;

    int x=10;
    pthread_create(&t1, NULL, worker, &x); //we send in reference? which void can point as it is generic pointer
    
    void* doubled;
    pthread_join(t1, &doubled); //DO NOT FORGET & as we need reference to VOID* NOT VOID* ITSELF

    int* doubledval=(int*)doubled;
    printf("MAIN: The doubled value returned: %d", *doubledval);
     free(doubledval); //parent needs to free the memory


    return 0;
}

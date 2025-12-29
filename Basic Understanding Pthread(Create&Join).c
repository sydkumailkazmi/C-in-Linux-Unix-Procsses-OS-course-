#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>



void* worker(void* arg){
printf("This is thread() worker \n");

return NULL; //pthread_exit(value) important to send a signal that thread is done // same as pthread_exit(NULL)
}


int main(){
 printf("This is main() \n");

 pthread_t t1;
 pthread_create(&t1,NULL,worker,NULL);
//if no join main() will not wait for its thread to execute and only main() printf will print and program will end
//would become zombie (wrong) bus uskay resources release nhi hongay

pthread_join(t1,NULL); // no returned value pointer so NULL



    return 0;
}
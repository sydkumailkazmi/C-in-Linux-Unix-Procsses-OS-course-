#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){

pid_t pid=fork();

if(pid<0){
printf("Fork failed. \n");

return 1;
}

if(pid==0){

printf("Child is executing.\n");
printf("The pid(return of fork()): %d \n", pid);
printf("The pid(the id of the process created): %d \n", getpid());
printf("The pid(the id of the parent): %d \n", getppid());

}
else if(pid>0){
wait(NULL);

printf("Parent is executing... \n");
printf("The pid(return of fork()): %d \n", pid);
printf("The pid(the id of the process created): %d \n", getpid());



}



return 0;
}

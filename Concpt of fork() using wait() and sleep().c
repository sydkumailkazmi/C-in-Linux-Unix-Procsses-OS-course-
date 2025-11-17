#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main(){

pid_t pid=fork();

printf("Program.. \n");

if(pid<0){
printf("Fork Failed \n");
}

if(pid==0)
{
printf("We are in child. \n");
printf("The return value of fork() in child:%d \n",pid);
printf("The pid of this child process:%d \n",getpid());
printf("The pid of this child process' parent:%d \n",getppid());

sleep(2);
}
else if(pid>0)
{
printf("We are in Parent. \n Waitinf for child to be executed.. \n");

wait(NULL);
printf("PARENT: child is executed now we executing parent. \n");
printf("The return value of fork() in parent:%d \n",pid);
printf("The pid of this parent process:%d \n",getpid());



}

return 0;
}

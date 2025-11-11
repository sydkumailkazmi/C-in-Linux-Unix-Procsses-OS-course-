#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
pid_t pid=fork();

if(pid<0){
printf("Fork Failed. \n");
return 1;
}

if(pid==0){
printf("CHILD: I am running! \n");
printf("(A 2 SEC PAUSE) \n");

sleep(2);

printf("CHILD: I am terminating! \n");

}
else if(pid>0)
{
printf("PARENT:I am waiting for my child. \n");
wait(0);
//child returns after exiting to this line
printf("My child has terminated now I am finishing. \n");

}
return 0;
}



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{

int fd[2]; 
if(pipe(fd)==-1)
{
printf("Error in pipe creation \n");
return 2;
}

pid_t pid=fork();

if(pid<0)
{
printf("Fork() error! \n");
return 3;
}

if(pid==0)
{
printf("We are in child. \n");
close(fd[0]);

int num;
printf("CHILD: Enter number:");
scanf("%d",&num);

write(fd[1],&num,sizeof(int));
close(fd[1]);

}
else if(pid>0)
{
printf("PARENT: Waiting for child to send: \n");
wait(NULL);
close(fd[1]);

int rec;

read(fd[0],&rec,sizeof(int));
close(fd[0]);

printf("PARENT:%d \n",rec);

}




return 0;
}

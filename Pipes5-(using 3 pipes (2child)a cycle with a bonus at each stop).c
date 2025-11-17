#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main(){

int fd1[2];
int fd2[2];
int fd3[2];

if(pipe(fd1)==-1 || pipe(fd2)==-1 ||pipe(fd3)==-1){
printf("Pipe Failed \n");
return 2;
}

pid_t pid1=fork();
if(pid1<0){
printf("Fork Failed \n");
return 3;
}

if(pid1==0){
//child1
close(fd1[1]);
close(fd2[0]);
close(fd3[1]);
close(fd3[0]);

int n;
read(fd1[0],&n,sizeof(int));
close(fd1[0]);

n=n*2;

write(fd2[1],&n,sizeof(int));
close(fd2[1]);

}
else if(pid1>0)
{
pid_t pid2=fork();
if(pid2<0){
printf("Fork Failed \n");
return 3;
}
if(pid2==0)
{
//child2
close(fd1[1]);
close(fd1[0]);
close(fd2[1]);
close(fd3[0]);

int n;
read(fd2[0],&n,sizeof(int));
close(fd2[0]);

n=n*2;

write(fd3[1],&n,sizeof(int));
close(fd3[1]);
}
else if(pid2>0)
{
//parent of both
close(fd1[0]);
close(fd3[1]);
close(fd2[1]);
close(fd2[0]);

int num;
printf("Enter a number to be multiplied by 4: ");
scanf("%d",&num);

write(fd1[1],&num,sizeof(int));
close(fd1[1]);

//waiting for both childs so it completes the cycle and gets 
waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

int mul;

read(fd3[0],&mul,sizeof(int));
close(fd3[0]);

printf("\n Your multiplied number in Parent: %d",mul);
}

}


return 0;
}

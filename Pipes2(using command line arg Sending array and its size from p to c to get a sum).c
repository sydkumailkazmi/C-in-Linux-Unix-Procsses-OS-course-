#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(int argc, char* argv[])
{

int size=argc-1;
int numarr[size];

int j=0;
for(int i=1; i<argc; i++) //at argv[0] we will have our program ./p2
{
numarr[j]=atoi(argv[i]);
j++;
}

int fd[2];

if(pipe(fd)==-1)
{
printf("Pipe creation failed. \n");
return 2;
}

pid_t pid=fork();

if(pid<0)
{
printf("fork creation failed. \n");
return 3;
}


if(pid>0){
printf("IN PARENT \n");
close(fd[0]);

printf("Sending size first \n");
write(fd[1],&size,sizeof(int));

printf("Sending array. \n");
write(fd[1],numarr,size*sizeof(int));

close(fd[1]);
wait(NULL);
}
else if(pid==0)
{
printf("IN CHILD \n");

close(fd[1]);

printf("Receiving size first \n");
int sz;

read(fd[0],&sz,sizeof(int));

int carr[sz];

printf("Recieving array. \n");

read(fd[0],carr,sz*sizeof(int));
close(fd[0]);

int sum=0;
printf("Array in Parent: ");
for(int i=0;i<sz; i++)
{
printf("%d, ",carr[i]);
sum=sum+carr[i];
}

printf("PARENT: SUM OF %d SIZE ARR IS: %d \n", sz,sum);
}


return 0;
}

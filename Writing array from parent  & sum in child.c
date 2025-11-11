#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(){

int fd[2];
if(pipe(fd)==-1)
{
printf("Pipe did not get created, error! \n");
return 2;
}

pid_t pid=fork();

if(pid<0){
printf("Fork Failed! \n");
return 3;
}

if(pid>0)
{
printf("-------------IN PARENT------------ \n");
close(fd[0]);

int list[]={3,4,5,1,3,9};
int size=6;
//we need to send size too to read
write(fd[1], &size, sizeof(int));
//Then send the array, no & needed
//BUT, imp is size* 
write(fd[1], list, size*sizeof(int));
close(fd[1]);

wait(NULL);
}

else if(pid==0){
close(fd[1]);

int siz;
read(fd[0],&siz,sizeof(int));
printf("Size recieved in child: %d \n", siz);
//it works in ubuntu without dynamic allocation using ubuntu
int reclist[siz];
read(fd[0],reclist,siz*sizeof(int));
close(fd[0]);

printf("List recieved in child: \n");
int sum=0;
for(int i=0; i<siz; i++)
{
printf("%d,",reclist[i]);
sum=sum+reclist[i];
}

printf("\n The sum of list: %d.",sum);

}

return 0;
}


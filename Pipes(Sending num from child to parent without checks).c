#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(){

int fd[2];

pipe(fd);

pid_t pid=fork();

if(pid==0){
printf("WE ARE IN CHILD \n");

close(fd[0]);

int x;
printf("Enter the number: ");
scanf("%d", &x);

write(fd[1], &x, sizeof(int));
close(fd[1]);

}
else if(pid>0){
wait(NULL);
close(fd[1]);

printf("WE ARE IN PARENT \n");
int y;

read(fd[0], &y, sizeof(int));
close(fd[0]);

printf("Printing in parent from child: %d",y);

}





return 0;
}

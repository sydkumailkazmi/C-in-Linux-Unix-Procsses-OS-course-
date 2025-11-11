#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(){

int fd1[2];//parent->child1
int fd2[2];//child1->child2
int fd3[2];//child2->parent

if(pipe(fd1)==-1 ||pipe(fd2)==-1 || pipe(fd3)==-1)
{
printf("Pipe creation failed! \n");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("Fork creation failed! \n");
return 3;
}

if(pid1==0)
{
close(fd1[1]);
close(fd2[0]);
close(fd3[0]);
close(fd3[1]);

//reading from parent
int num;
read(fd1[0],&num,sizeof(int));
close(fd1[0]);

num=num+5;

//writing in child2
write(fd2[1],&num,sizeof(int));
close(fd2[1]);

}
else
{
//it is a parent
pid_t pid2=fork();

if(pid2<0)
{
printf("Fork creation failed! \n");
return 3;
}

if(pid2==0)
{
//it is child2

close(fd1[0]);
close(fd1[1]);
close(fd2[1]);
close(fd3[0]);

int n;

//reading from child1
read(fd2[0],&n,sizeof(int));
close(fd2[0]);

n=n+5;
//writing in parent
write(fd3[1],&n,sizeof(int));
close(fd3[1]);
}

else
{
//we are in parent of both
close(fd2[1]);
close(fd2[0]);
close(fd1[0]);
close(fd3[1]);
//the intial number we want to write

int numb;
printf("Enter any number:");
scanf("%d", &numb);

//writing in child1 first
write(fd1[1],&numb,sizeof(int));
close(fd1[1]);

int final;
//reading the final outcome from child2
read(fd3[0],&final,sizeof(int));
close(fd3[0]);

//we need children to finish their and work and both of them

waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);


printf("Your final outcome after 2 increments of 5 in Parent: %d", final);
}


}





return 0;
}

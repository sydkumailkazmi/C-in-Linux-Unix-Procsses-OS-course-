#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main(int argc, char*argv[])
{
int size=argc-1;
int numarr[size];
int j=0;
for(int i=1;i<argc;i++)
{
numarr[j]=atoi(argv[i]);
j++;
}
int fhalf,shalf;
if(size%2==0)
{
fhalf=size/2;
shalf=size/2;
}
else
{
fhalf=size/2;
shalf=size-(size/2);
}



int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];

if(pipe(fd1)==-1||pipe(fd2)==-1||pipe(fd3)==-1||pipe(fd4)==-1)
{
printf("pipe error");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("fork error");
return 3;
}

if(pid1==0)
{
//child1
close(fd4[1]);
close(fd4[0]);
close(fd2[1]);
close(fd2[0]);
close(fd1[1]);
close(fd3[0]);

int farr[fhalf];
read(fd1[0],farr,sizeof(int)*fhalf);
close(fd1[0]);

double mean;
double sum=0;
for(int i=0; i<fhalf; i++)
{
sum=sum+farr[i];
}
mean=sum/fhalf;

int c1=getpid();

write(fd3[1],&c1,sizeof(int));
write(fd3[1],&mean,sizeof(double));
close(fd3[1]);
}
else if(pid1>0)
{
pid_t pid2=fork();
if(pid2==0)
{
//child2
close(fd3[1]);
close(fd3[0]);
close(fd1[1]);
close(fd1[0]);
close(fd2[1]);
close(fd4[0]);

int farr[shalf];
read(fd2[0],farr,sizeof(int)*shalf);
close(fd2[0]);

double mean;
double sum=0;
for(int i=0; i<shalf; i++)
{
sum=sum+farr[i];
}
mean=sum/shalf;

int c2=getpid();

write(fd4[1],&c2,sizeof(int));
write(fd4[1],&mean,sizeof(double));
close(fd4[1]);

}
else if(pid2>0)
{
close(fd1[0]);
close(fd3[1]);
close(fd2[0]);
close(fd4[1]);



write(fd1[1],numarr,sizeof(int)*fhalf);
close(fd1[1]);

write(fd2[1],numarr+fhalf,sizeof(int)*shalf); //starting address would be from where first half ended till shalf size
close(fd2[1]);

waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

int pid1;
double mean1;

read(fd3[0],&pid1,sizeof(int));
read(fd3[0],&mean1,sizeof(double));
close(fd3[0]);

int pid2;
double mean2;

read(fd4[0],&pid2,sizeof(int));
read(fd4[0],&mean2,sizeof(double));
close(fd4[0]);

printf("\nFirst Half of the Array: ");
for(int i=0;i<fhalf;i++)
{
printf("%d, ",numarr[i]);
}

printf("\nSecond Half of the Array: ");

for(int i=fhalf;i<size;i++)
{
printf("%d, ",numarr[i]);
}

if(mean1>mean2)
{
printf("\n The child with pid- %d has greater mean of %lf",pid1,mean1);
}
else
{
printf("\n The child with pid- %d has greater mean of %lf",pid2,mean2);
}

}

}
return 0;
}

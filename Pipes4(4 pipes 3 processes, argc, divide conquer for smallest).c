#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(int argc, char*argv[])
{
int size=argc-1;
int numarr[size];

int j=0;
for(int i=1; i<argc; i++)
{
numarr[j]=atoi(argv[i]);
j++;
}

int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];


if(pipe(fd1)==-1||pipe(fd2)==-1||pipe(fd3)==-1||pipe(fd4)==-1)
{
printf("pipe failed");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("fork failed");
return 3;
}

//child1
if(pid1==0)
{

close(fd1[1]);
close(fd3[0]);

int sz1=size/2;
int arr[sz1];

read(fd1[0],arr,sizeof(int)*sz1);
close(fd1[0]);

for(int i=0; i<sz1; i++)
{ 
printf("\n first half in child1: %d,",arr[i]);
}

int smallest=arr[0];
for(int i=0;i<sz1; i++)
{
if(arr[i]<smallest)
{
smallest=arr[i];
}
}
printf("\n smallest gone from child1: %d",smallest);


write(fd3[1],&smallest,sizeof(int));
close(fd3[1]);

}
else if(pid1>0)
{
pid_t pid2=fork();

if(pid2<0)
{
printf("fork failed");
return 3;
}

if(pid2==0)
{
//child2
close(fd2[1]);
close(fd4[0]);

int sz1=size/2;
int arr[sz1];

read(fd2[0],arr,sizeof(int)*sz1);
close(fd2[0]);

for(int i=0; i<sz1; i++)
{
printf("\n first half in child2: %d,",arr[i]);
}

int smallest=arr[0];
for(int i=0;i<sz1; i++)
{
if(arr[i]<smallest)
{
smallest=arr[i];
}
}

printf("\n smallest gone from child2: %d",smallest);

write(fd4[1],&smallest,sizeof(int));
close(fd4[1]);




}
else if(pid2>0)
{
//in parent

close(fd1[0]);
close(fd2[0]);
close(fd3[1]);
close(fd4[1]);

int size1=size/2;
int fhalf[size1];


for(int i=0; i<size1; i++)
{
fhalf[i]=numarr[i];
}

write(fd1[1],fhalf,size1*sizeof(int));
close(fd1[1]);

int shalf[size1];

int j=0;
for(int i=size1; i<size; i++)
{
shalf[j]=numarr[i];
j++;
}

write(fd2[1],shalf,size1*sizeof(int));
close(fd2[1]);

waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

int small1;
read(fd3[0],&small1,sizeof(int));
close(fd3[0]);


int small2;
read(fd4[0],&small2,sizeof(int));
close(fd4[0]);


if(small1<small2)
{
printf("\n PARENT:smallest num-> %d \n",small1);
}
else
{
printf("\n PARENT:smallest num-> %d \n",small2);

}

}





}






return 0;
}

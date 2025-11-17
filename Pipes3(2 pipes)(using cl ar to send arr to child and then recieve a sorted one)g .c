#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main(int argc, char* argv[])
{

int fd1[2];
int fd2[2];

if(pipe(fd1)==-1||pipe(fd2)==-1)
{
printf("Pipe not created \n");
return 2;
}

pid_t pid=fork();
if(pid<0)
{
printf("Fork not created \n");
return 3;
}

if(pid>0)
{
int size=argc-1;
int numarr[size];

int j=0;
for(int i=1;i<argc;i++)
{
numarr[j]=atoi(argv[i]);
j++;
}

close(fd1[0]);
close(fd2[1]);

write(fd1[1],&size,sizeof(int));

write(fd1[1],numarr,size*sizeof(int));
close(fd1[1]);

wait(NULL);

int sortedarr[size];

read(fd2[0],sortedarr,size*sizeof(int));
close(fd2[0]);


printf("Parent: Sorted Array->");
for(int i=0;i<size;i++)
{
printf("%d,",sortedarr[i]);
}

}
else if(pid==0){
close(fd1[1]);
close(fd2[0]);

int sz;
read(fd1[0],&sz,sizeof(int));

int arr[sz];
read(fd1[0],arr,sizeof(int)*sz);
close(fd1[0]);

for(int i=0; i<sz-1; i++)
{
for(int j=0; j<sz-1-i; j++)
{
if(arr[j]>arr[j+1])
{
int temp=arr[j];
arr[j]=arr[j+1];
arr[j+1]=temp;
}
}

}

write(fd2[1],arr,sz*sizeof(int));
close(fd2[1]);
}


return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main()
{

int n;
printf("How many numbers you want to in your list(even): ");
scanf("%d", &n);

int list[n];

printf("Enter Elements in your list: ");
for(int i=0;i<n;i++)
{
scanf("%d", &list[i]); 
}

int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];

if(pipe(fd1) == -1 ||pipe(fd2) == -1 ||pipe(fd3) == -1 ||pipe(fd4) == -1 )
{
printf("Pipe failed. \n");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("Fork failed. \n");
return 3;
}

if(pid1==0)
{
//in child1

close(fd2[1]);
close(fd2[0]);
close(fd4[0]);
close(fd4[1]);
close(fd1[1]);
close(fd3[0]);

//reading frist half list from parent
int fhalf[n/2];
read(fd1[0],fhalf,sizeof(int)*(n/2));
//wait to read permisson from parent
int perm;

read(fd1[0],&perm,sizeof(int));

if(perm==1)
{
//finding smallest in first half
int smallest = fhalf[0];

for(int i=0; i<n/2; i++)
{
if(fhalf[i]<smallest)
{
smallest=fhalf[i];
}

}
printf("CHILD1: smallest=%d \n", smallest);
//writing that smallest back to parent

write(fd3[1], &smallest, sizeof(int));
close(fd3[1]);

//but cant exit still without permission
int perm2;
read(fd1[0],&perm2,sizeof(int));

close(fd1[0]);
if(perm2==1)
{
exit(0);
}
else
{
sleep(10);
}

}
else
{

}


}
else
{
pid_t pid2=fork();

if(pid2<0)
{
printf("Fork failed. \n");
return 4;
}

if(pid2==0)
{
//in child2
close(fd3[1]);
close(fd3[0]);
close(fd1[0]);
close(fd1[1]);
close(fd2[1]);
close(fd4[0]);

//read from parent

int Shalf[n/2];

read(fd2[0],Shalf,sizeof(int)*(n/2));
//wait to read permisson from parent
int perm;

read(fd2[0],&perm,sizeof(int));



if(perm==1)
{

//finding smallest in first half
int small = Shalf[0];

for(int i=0; i<n/2; i++)
{
if(Shalf[i]<small)
{
small=Shalf[i];
}

}

printf("CHILD2: smallest=%d \n", small);


//sending it to parent

write(fd4[1],&small,sizeof(int));
close(fd4[1]);

//but cant exit still without permission
int perm2;
read(fd2[0],&perm2,sizeof(int));

close(fd2[0]);
if(perm2==1)
{
exit(0);
}
else
{
sleep(10);
}

}
else
{

}

}
else
{
// we are in parent
close(fd4[1]);
close(fd2[0]);
close(fd1[0]);
close(fd3[1]);


int half=n/2;

//writing first half to c1
write(fd1[1],list,sizeof(int)*half);

//giving permisson
int per=1;
write(fd1[1],&per,sizeof(int));

//giving permisson to exit
write(fd1[1],&per,sizeof(int));

close(fd1[1]);

//writing second half to c2 (+half to move the address to half start)
write(fd2[1],list+half,sizeof(int)*half);
//giving permisson
per=1;
write(fd2[1],&per,sizeof(int));


//giving permisson to exit

write(fd2[1],&per,sizeof(int));

close(fd2[1]);

//reading from c1 small1
int small1;
read(fd3[0],&small1,sizeof(int));
close(fd3[0]);


//reading from c1 small1
int small2;
read(fd4[0],&small2,sizeof(int));
close(fd4[0]);

//waiting jb tk dono child wapis small nhi dey deti
waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

//smallest in list
if(small1<small2)
{
printf("PARENT: The final smallest number in whole list: %d \n",small1);
}
else
{
printf("PARENT: The final smallest number in whole list: %d \n",small2);
}
}


}

return 0;
}

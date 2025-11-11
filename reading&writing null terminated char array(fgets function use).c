#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>


int main()
{
int fd[2];

if(pipe(fd)==-1){
printf("Pipe creation Failed! \n");
return 2;
}

pid_t pid=fork();
if(pid<0)
{
printf("Fork Failed. \n");
return 3;
}

if(pid==0){
close(fd[0]);

char line[200];
printf("Enter a line:");
//this allows to get string input , stdin means from keyboard
fgets(line,200,stdin);

//but it is /n terminated so w e make it null terminated
line[strlen(line)-1]='\0';

int length=strlen(line);
write(fd[1],&length,sizeof(int));

//each char is of size of 1 so need of sizeof(char)*strlen(line)
int byt;
byt=write(fd[1],line,strlen(line));
close(fd[1]);

printf("Bytes of data written in child: %d \n",byt);

}
else if(pid>0)
{
wait(NULL);
close(fd[1]);

int len;
int bt;

read(fd[0], &len, sizeof(int));
char str[len];

bt=read(fd[0],str,len);
close(fd[0]);

printf("Bytes of data read in parent: %d \n",bt);

printf("The recieved string: ");
for(int i=0; i<len;  i++){
printf("%c",str[i]);
}

}
return 0;
}

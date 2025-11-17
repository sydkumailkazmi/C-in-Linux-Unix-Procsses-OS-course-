#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main()
{

int fd1[2];
int fd2[2];
int fd3[2];


if(pipe(fd1)==-1||pipe(fd2)==-1||pipe(fd3)==-1)
{
printf("Pipe creation error!");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("Fork creation error!");
return 2;
}

if(pid1==0){
close(fd1[1]);
close(fd2[0]);
close(fd3[0]);
close(fd3[1]);


int len;
read(fd1[0],&len,sizeof(int));

char word[len+1];
read(fd1[0],word,sizeof(char)*len);
close(fd1[0]);

char reverse[len+1];

int j=0;
for(int i=len-1;i>=0;i--)
{
reverse[j]=word[i];
j++;
}

printf("\n To check in child1 reversed: %s",reverse);

write(fd2[1],&len,sizeof(int));

write(fd2[1],word,sizeof(char)*len);

write(fd2[1],reverse,sizeof(char)*len);
close(fd2[1]);

}
else if(pid1>0)
{
pid_t pid2=fork();

if(pid2<0)
{
printf("Fork creation error!");
return 2;
}

if(pid2==0)
{
//child2

close(fd1[1]);
close(fd1[0]);
close(fd2[1]);
close(fd3[0]);

int size;
read(fd2[0],&size,sizeof(int));
printf("\n To check in child2 size: %d",size);

char orig[size+1];
read(fd2[0],orig,sizeof(char)*size);
printf("\n To check in child2 orignal: %s",orig);

char rev[size+1];
read(fd2[0],rev,sizeof(char)*size);
close(fd2[0]);
printf("\n To check in child2 reversed: %s",rev);

int palindrome=0;
for(int i=0; i<size; i++)
{
if(rev[i]!=orig[i])
{
palindrome=0;
break;
}
else{
palindrome=1;
}
}

write(fd3[1],&palindrome,sizeof(int));
close(fd3[1]);

}
else if(pid2>0)
{
//in parent
close(fd1[0]);
close(fd2[0]);
close(fd2[1]);
close(fd3[1]);

char input[200];
printf("\n Parent: Enter the word you want to check palindrome of-> ");
fgets(input,200,stdin);
input[strlen(input)-1]='\0';

int leng=strlen(input);
printf("\n To check in parent size: %d",leng);
write(fd1[1],&leng,sizeof(int));

write(fd1[1],input,sizeof(char)*leng); //sirf leng tk bhejna pura thori bhejna
close(fd1[1]);

waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

int checkpalin;

read(fd3[0],&checkpalin,sizeof(int));
close(fd3[0]);


if(checkpalin==1){
printf("\n You word is palindrome \n");
}
else
{
printf("\n You word is not palindrome \n");
}

}


}



return 0;
}

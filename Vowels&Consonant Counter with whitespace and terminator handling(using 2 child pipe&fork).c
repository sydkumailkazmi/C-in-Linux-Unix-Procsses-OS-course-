#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main()
{
char input[100];
printf("\nEnter the word you want to check consonants and vowels of: ");
fgets(input,100,stdin);
input[strlen(input)-1]='\0';
//scanf("%s",input); wont read after space 

int length=strlen(input); //always after putting null yourself wrna \n inko bhi ginta woh
//length is without \0

int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];

if(pipe(fd1)==-1 ||pipe(fd2)==-1 ||pipe(fd3)==-1 ||pipe(fd4)==-1)
{
printf("Pipe failed!");
return 2;
}

pid_t pid1=fork();

if(pid1<0)
{
printf("fork failed!");
return 3;
}

if(pid1==0)
{
//child1
close(fd2[1]);
close(fd2[0]);
close(fd4[1]);
close(fd4[0]);
close(fd1[1]);
close(fd3[0]);

char word[length+1]; //length mein '\0' include nhi tha but read humnay utna hi krna

read(fd1[0],word,sizeof(char)*length);
close(fd1[0]);

int vowels=0;
for(int i=0;i<length;i++)
{
if((word[i]=='A' ||word[i]=='E' ||word[i]=='I' ||word[i]=='O' ||word[i]=='U' )||(word[i]=='a' ||word[i]=='e' ||word[i]=='i' ||word[i]=='o' ||word[i]=='u'))
{
vowels++;
}
}

write(fd3[1],&vowels,sizeof(int));
close(fd3[1]);

}
else if(pid1>0)
{
pid_t pid2=fork();
if(pid2<0)
{
printf("fork failed!");
return 3;
}
if(pid2==0)
{
//child2
close(fd1[1]);
close(fd1[0]);
close(fd3[1]);
close(fd3[0]);
close(fd2[1]);
close(fd4[0]);

char word[length+1]; //length mein '\0' include nhi tha but read humnay utna hi krna

read(fd2[0],word,sizeof(char)*length);
close(fd2[0]);

int cons=0;
int extra=0;
for(int i=0;i<length;i++)
{
if((word[i]=='A' ||word[i]=='E' || word[i]==' '||word[i]=='I' ||word[i]=='O' ||word[i]=='U')||(word[i]=='a' ||word[i]=='e' ||word[i]=='i' ||word[i]=='o' ||word[i]=='u'))
{
extra++;
//if space toh woh bhi ignore krey for double words
}
else
{
cons++;
}
}

write(fd4[1],&cons,sizeof(int));
close(fd4[1]);
}

else if(pid2>0)
{
//parent
close(fd1[0]);
close(fd2[0]);
close(fd3[1]);
close(fd4[1]);

write(fd1[1],input,sizeof(char)*length);
close(fd1[1]);

write(fd2[1],input,sizeof(char)*length);
close(fd2[1]);

waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);

int vowelsC;
read(fd3[0],&vowelsC,sizeof(int));
close(fd3[0]);

int consonantC;
read(fd4[0],&consonantC,sizeof(int));
close(fd4[0]);

printf("\n The vowels in word %s:%d",input,vowelsC);
printf("\n The consonant in word %s:%d",input,consonantC);
}
}



return 0;
}

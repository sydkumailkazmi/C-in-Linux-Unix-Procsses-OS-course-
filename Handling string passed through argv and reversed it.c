#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main(int argc, char*argv[])
{
if(argc<2)
{
printf("Not enough arguments.");
return 2;
}
char word[100];

printf("\n Printing directly using argv[1]: %s",argv[1]);
strcpy(word,argv[1]);
printf("\n Printing word: %s",word);
int len=strlen(word); 
printf("\n Length of word w/o null terminator using strlen():%d",len); 

char reverse[len+1]; //len doesnt count null terminator

int j=0; //len starts with 1 so need to -1 to start from last 0-based index
for(int i=len-1; i>=0; i--)
{
reverse[j]=word[i];
j++;
}

printf("\n The reversed string: %s \n",reverse);

return 0;
}

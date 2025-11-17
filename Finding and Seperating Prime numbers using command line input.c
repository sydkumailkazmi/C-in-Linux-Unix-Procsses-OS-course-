#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main(int argc, char* argv[])
{
int size=argc-1;
int numarr[size];
int j=0;
for(int i=1; i<argc;i++)
{
numarr[j]=atoi(argv[i]);
j++;
}

printf("\n You array: ");
for(int i=0; i<size;i++)
{
printf("%d,",numarr[i]);
}

int divided=0;
int primecount=0;
for(int i=0; i<size; i++)
{
divided=0;
for(int j=2; j<=numarr[i]; j++)
{
if(numarr[i]%j==0)
{
divided++;
}

}
if(divided==1)
{
printf("\n %d is prime.",numarr[i]);
primecount++;
}

}

printf("\n Total %d are prime.",primecount);


return 0;
}

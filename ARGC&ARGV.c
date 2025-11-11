#include<stdio.h>

int main(int argc, char* argv[]) 
{

printf("No. of arguments you gave are %d. \n ",argc);
for(int i=0; i<argc; i++)
{
printf("Number-%d:%s \n ",i,argv[i]);
}




return 0;
}

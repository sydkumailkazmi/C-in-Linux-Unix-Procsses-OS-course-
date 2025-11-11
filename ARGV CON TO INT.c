#include<stdlib.h>
#include<stdio.h>


int main(int argc, char* argv[])
{
int num1=atoi(argv[1]);
int num2=atoi(argv[2]);
int product=num1*num2;

printf("The product of %s and %s is:%d \n",argv[1],argv[2],product);

return 0;
}

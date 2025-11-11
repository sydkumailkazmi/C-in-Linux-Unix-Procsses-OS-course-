#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>

int main(){

pid_t pid=fork();

if(pid<0)
{
printf("Fork Failed! \n");
return 1;
}

if(pid>0){
wait(NULL);

printf("PARENT: Child process finished. \n");


printf("We are in PARENT: \n");
printf("Give me 2 numbers to add:");
int num1;
int num2;
scanf("%d %d",&num1,&num2);
int sum=num1+num2;
printf("The sum of %d and %d is: %d \n",num1, num2, sum);




}
else if(pid==0){
printf("We are in CHILD: \n");
execlp("./con","con","10","12", NULL);

printf("Exec did not work \n");
exit(1);
}




return 0;
}


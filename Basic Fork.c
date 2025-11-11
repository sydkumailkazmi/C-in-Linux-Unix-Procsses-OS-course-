#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
pid_t pid=fork();

printf("Return value from Fork(): %d \n", pid);

if(pid==0){
    printf("It is a child.\n");
    printf("PID child: %d \n", getpid());
}
else if(pid>0){
    printf("It is a parent process. \n");
    printf("PID parent: %d \n", getpid());
}

return 0;
}

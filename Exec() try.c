#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

//check path of desktop using pwd 
// /home/ubuntu/Desktop

//execlp("/home/ubuntu/Desktop/g1","g1","1","2","3","4",NULL);

//execl("./g1","g1","1","2","3","4",NULL);

//execlp("~/Desktop/g1.c","g1.c", "1","2","3","4",NULL);
//execlp("ls","ls","-l",NULL);
printf("Exec failed");

return 0;
}

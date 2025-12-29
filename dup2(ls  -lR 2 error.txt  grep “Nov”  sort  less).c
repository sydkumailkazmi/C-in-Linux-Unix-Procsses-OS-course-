#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
    int p1[2], p2[2], p3[2];

    pipe(p1);  
    pipe(p2);  
    pipe(p3);  

    
    if (fork() == 0)
    {
        
        dup2(p1[1], 1);

        int err = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(err, 2);

        
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);
        close(err);

        execlp("ls", "ls", "/", "-lR", NULL);
        perror("exec ls");
        exit(1);
    }

   
    if (fork() == 0)
    {
        dup2(p1[0], 0);

       
        dup2(p2[1], 1);

        
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("grep", "grep", "Nov", NULL);
        perror("exec grep");
        exit(1);
    }

    if (fork() == 0)
    {
       
        dup2(p2[0], 0);

        dup2(p3[1], 1);

      
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("sort", "sort", NULL);
        perror("exec sort");
        exit(1);
    }

  
    if (fork() == 0)
    {
       
        dup2(p3[0], 0);

        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("less", "less", NULL);
        perror("exec less");
        exit(1);
    }

   
    close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    close(p3[0]); close(p3[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}

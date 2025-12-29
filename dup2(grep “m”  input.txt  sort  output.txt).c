#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() 
{
    int fd[2];

    if (pipe(fd) == -1) {
        printf("Pipe failed!\n");
        return 1;
    }

    pid_t p1 = fork();

    if (p1 < 0) {
        printf("Fork 1 failed!\n");
        return 2;
    }

    if (p1 == 0)
    {
        

        close(fd[0]);    

      
        dup2(fd[1], 1);

        close(fd[1]);

      
        int file = open("input.txt", O_RDONLY);
        if (file < 0) {
            printf("Could not open input.txt\n");
            exit(3);
        }

        dup2(file, 0);
        close(file);

        execlp("grep", "grep", "m", NULL);

        printf("exec failed in child 1\n");
        exit(4);
    }

  

    pid_t p2 = fork();

    if (p2 < 0) {
        printf("Fork 2 failed!\n");
        return 3;
    }

    if (p2 == 0)
    {
       

        close(fd[1]);    

        dup2(fd[0], 0);
        close(fd[0]);

       
        int file = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file < 0) {
            printf("Could not open output.txt\n");
            exit(5);
        }

       
        dup2(file, 1);
        close(file);

        execlp("sort", "sort", NULL);

        printf("exec failed in child 2\n");
        exit(6);
    }

    // ===== PARENT =====
    close(fd[0]);
    close(fd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    return 0;
}

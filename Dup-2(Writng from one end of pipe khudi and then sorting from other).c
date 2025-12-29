#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        printf("Pipe error.\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Fork failed.\n");
        return 2;
    }

    if (pid == 0)
    {
        // ------------ CHILD PROCESS ------------

        // We only need to READ from pipe → so close write end
        close(fd[1]);

        // redirect child's STDIN (0) → to pipe's read end (fd[0])
        dup2(fd[0], 0);

        // original fd is no longer needed
        close(fd[0]);

        // Now run "sort" program
        // sort reads from STDIN → but STDIN is now pipe input
        execlp("sort", "sort", NULL);

        printf("Error running sort.\n");
        exit(1);
    }
    else
    {
        // ------------ PARENT PROCESS ------------

        // We only WRITE → so close read end
        close(fd[0]);

        char data[] = "banana\napple\ncherry\n";

        write(fd[1], data, strlen(data));

        // IMPORTANT → tell child no more data is coming
        close(fd[1]);

        wait(NULL);
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    if (pid == 0) {
       

        int fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            perror("open");
            return 1;
        }

        dup2(fd, 1);  
        close(fd);     

        execlp("ls", "ls", "/", "-l", NULL);

        perror("exec failed");
        return 2;
    }

   
    wait(NULL);
    printf("Output written to input.txt\n");

    return 0;
}

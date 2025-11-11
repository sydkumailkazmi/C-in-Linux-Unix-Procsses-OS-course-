// fork.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <num1> <num2>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if(pid == 0) {
        // Child process: execute con.c using execlp
        printf("CHILD: Executing con.c program\n");
        execlp("./con", "con", argv[1], argv[2], NULL);

        // If execlp fails
        perror("execlp failed");
        exit(1);
    } 
    else {
        // Parent process: multiply numbers itself
        int num1 = atoi(argv[1]);
        int num2 = atoi(argv[2]);
        int product = num1 * num2;
        printf("PARENT: Product of %d and %d is %d\n", num1, num2, product);

        // Wait for child to finish
        wait(NULL);
        printf("PARENT: Child process finished.\n");
    }

    return 0;
}

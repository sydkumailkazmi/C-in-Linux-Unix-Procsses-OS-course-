int fd[2];
pipe(fd);

pid_t p1 = fork();

if (p1 == 0) {
    // CHILD 1 → ls → writes to pipe
    dup2(fd[1], 1);   // redirect stdout to pipe write end
    close(fd[0]);
    close(fd[1]);
    execlp("ls", "ls", NULL);
}

pid_t p2 = fork();

if (p2 == 0) {
    // CHILD 2 → sort → reads from pipe
    dup2(fd[0], 0);   // redirect stdin to pipe read end
    close(fd[1]);
    close(fd[0]);
    execlp("sort", "sort", NULL);
}

// parent closes both ends
close(fd[0]);
close(fd[1]);

wait(NULL);
wait(NULL);

//*** Child-to-Parent pipe ***//
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int pid;
    int fd[2];
    pipe(fd);
    pid = fork();
    if (pid < 0 ) {
        exit(-1);
    } else if (pid == 0) {
        close (1);
        dup(fd[1]);
        close(fd[1]);
        close(fd[0]);
        execl("./pre", "./pre", (char*)0);
    } else {
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        execl("./sort", "./sort", (char*)0);
    }
    return 0;
}

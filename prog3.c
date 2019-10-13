//*** Command line program ***//
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
    for (int i = 0; i < argc -1; ++i)
        strcpy(argv[i], argv[i+1]);
    argv[argc -1] = (char*)0;
    
    int pid;
    pid = fork();

    if (pid < 0) {
        exit(-1);
    } else if (pid == 0) {
        printf("Child PID: %d\nParent PID : %d\n", getpid(), getppid());
        execvp(argv[0], argv);
    }
    else
        wait(0);
    return 0;
}

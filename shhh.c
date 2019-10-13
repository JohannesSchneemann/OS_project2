//*** shell ***//
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int index = 0, status, middle = 0, cont = 0, pid;
    int rfd[2], lfd[2], inputFile = -1, outputfile = -1;
    const int MaxBufferLength = 80;
    const int MaxCmdLength = 20;
    char character, buffer[MaxBufferLength], *ptr,
         *argv[MaxCmdLength];
   
    while (1) {
        ptr = buffer;

        printf ("shhh>");

        while ((character = getchar()) != '\n' || cont) {
            if (character == ' ') {
                if (middle) {
                    middle = 0;
                    *(ptr++) = 0;
                }
            } else if (character == '\n') {
                cont = 0;
            } else if (character == '\\' && !middle) {
                cont = 1; }
            else {
                if (!middle) {
                    middle = 1;
                    argv[index++] = ptr;
                    *(ptr++) = character;
                } else {
                    *(ptr++) = character;
                }
            }
        }
        *(ptr++) = 0;
        argv[index] = 0;

        if (strcmp(argv[0], "exit") == 0) {
            exit(0); }

        // remove any special characters
        int pipeCounter = 0, cmdCounter = 0, pipes[10], commands[10],
            locationOutputFile = -1, locationInputFile = -1; commands[cmdCounter++] = 0;
        
        // check argv for equal, input & output redirection
        for (int i = 1; i < index; ++i) {
            if (strcmp(argv[i], "|") == 0) {
                pipes[pipeCounter++] = i;
                argv[i] = 0;
                commands[cmdCounter++] = i + 1;
            }
            else if (strcmp(argv[i], ">") == 0) {
                locationOutputFile = i + 1;
                argv[i] = 0;
            }
            else if (strcmp(argv[i], "<") == 0) {
                locationInputFile = i + 1;
                argv[i] = 0;
            }
        }
    
        for (int command = 0; command <= pipeCounter; ++command) {
            if (command < pipeCounter) {
                pipe(rfd);
            }
            pid = fork();
            // error message if failed
            if (pid < 0) {
                exit(-1);
            }
            // child process
            else if (pid == 0) {
                if (command == 0 && locationInputFile >= 0) { //redirect input
                    inputFile = open(argv[locationInputFile], O_RDONLY); // read file
                    if (inputFile < 0) {
                        exit(-1);
                    }
                    close(0);
                    dup(inputFile);
                    close(inputFile);
                }
                if (pipeCounter > 0) {
                    // 1st cmd (write only)
                    if (command == 0) {
                        close(1);
                        dup(rfd[1]);
                        close(rfd[1]);
                        close(rfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                    }
                    // middle cmd (read & write)
                    else if (command < pipeCounter) {
                        close(0);
                        dup(lfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                    }
                    // last cmd (read only)
                    else {
                        close(0);
                        dup(lfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                        close(rfd[0]);
                        close(rfd[1]);
                    }
                }

                // redirect output
                if (command == locationOutputFile >= 0 && pipeCounter) {
                    outputfile = open(argv[locationOutputFile], O_CREAT | O_WRONLY, 0754);
                    if (outputfile < 0) {
                        exit(-1);
                    }
                    close(1);
                    dup(outputfile);
                    close(outputfile);
                }
                execvp(argv[commands[command]], &argv[commands[command]]);
                printf(" Didn't execute! \n ");
            }
            // parent process
            else {
                // move & close pipes
                if (command > 0) {
                    close(lfd[0]);
                    close(lfd[1]);
                }
                lfd[0] = rfd[0];
                lfd[1] = rfd[1];
                wait(&status);
            }
        }
    }
}

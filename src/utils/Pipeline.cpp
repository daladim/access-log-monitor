// Inspired from https://stackoverflow.com/questions/9405985/linux-3-0-executing-child-process-with-piped-stdin-stdout
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

#include "Pipeline.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

bool execWithStdin(const char* command, char* const args[], char* const env[], const char* stdin){
    int aStdinPipe[2];
    int aStdoutPipe[2];
    int pidChild;
    char nChar;
    int nResult;

    if (pipe(aStdinPipe) < 0) {
        perror("allocating pipe for child input redirect");
        return -1;
    }
    if (pipe(aStdoutPipe) < 0) {
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
        return -1;
    }

    pidChild = fork();
    if (0 == pidChild) {
        // child continues here

        // redirect stdin
        if (dup2(aStdinPipe[PIPE_READ], STDIN_FILENO) == -1) {
            exit(errno);
        }

        // redirect stdout
        if (dup2(aStdoutPipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
            exit(errno);
        }

        // redirect stderr
        if (dup2(aStdoutPipe[PIPE_WRITE], STDERR_FILENO) == -1) {
            exit(errno);
        }

        // all these are for use by parent only
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);

        // run child process image
        // replace this with any exec* function find easier to use ("man exec")
        nResult = execve(command, args, env);

        // if we get here at all, an error occurred, but we are in the child
        // process, so just exit
        return false;


    } else if (pidChild > 0) {
        // parent continues here

        // close unused file descriptors, these are for child only
        close(aStdinPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);

        // Include error check here
        if (NULL != stdin) {
            write(aStdinPipe[PIPE_WRITE], stdin, strlen(stdin));
        }
        close(aStdinPipe[PIPE_WRITE]);

        // Just a char by char read here, you can change it accordingly
        while (read(aStdoutPipe[PIPE_READ], &nChar, 1) == 1) {
            write(STDOUT_FILENO, &nChar, 1);
        }
        close(aStdoutPipe[PIPE_READ]);
        // You could keep the pipes open as long as you want to talk to the child

        return true;

    } else {
        // failed to create child
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);
    }
    return false;
}

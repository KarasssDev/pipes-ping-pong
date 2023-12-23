#include "communication.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void create_pipe(int* inputPipe, int* outputPipe) {
    int pipes[2];

    if(pipe(pipes)) {
        perror("Failed to create pipes\n");
        exit(1);
    }

    *inputPipe = pipes[0];
    *outputPipe = pipes[1];
}

void send_message(int outputPipe, const char* buffer, size_t size) {
    int nBytes = write(outputPipe, buffer, size);
    if (nBytes == -1 || nBytes != size) {
        printf("Send failed\n");
        exit(1);
    }
}

void receive_message(int inputPipe, char* buffer, size_t size) {
    int nBytes = read(inputPipe, buffer, size);
    if (nBytes == -1 || nBytes != size) {
        printf("Receive failed\n");
        exit(1);
    }
}
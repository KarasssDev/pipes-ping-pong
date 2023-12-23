#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "communication.h"


void receive_concrete_message(const char* msg, size_t size, int inputPipe) {
    char* buffer = malloc(size);

    receive_message(inputPipe, buffer, size);

    if (strcmp(buffer, msg) != 0) {
        free(buffer);
        exit(1);
    }
    free(buffer);
}


void run_ping_pong(
    pid_t pid,
    int maxReplyCount,
    int inputPipe,
    int outputPipe,
    const char* inputMessage,
    const char* outputMessage
    ) {

    printf("%s process[%i] started\n", outputMessage, pid);

    size_t outputMessageSize = 5;
    size_t inputMessageSize = 5;

    int isPing = strcmp(outputMessage, "ping") == 0 && strcmp(inputMessage, "pong") == 0;
    int isPong = strcmp(outputMessage, "pong") == 0 && strcmp(inputMessage, "ping") == 0;

    if (!isPing && !isPong) {
        printf("Only ping or pong supported\n");
        exit(1);
    }

    int replyCount = 0;

    if (isPing) {
        send_message(outputPipe, outputMessage, outputMessageSize);
        printf("process[%i] send: %s\n", pid, outputMessage);
    }


    while(replyCount < maxReplyCount - 1) {
        replyCount++;
        receive_concrete_message(inputMessage, inputMessageSize, inputPipe);
        printf("process[%i] received: %s\n", pid, inputMessage);
        send_message(outputPipe, outputMessage, outputMessageSize);
        printf("process[%i] send: %s\n", pid, outputMessage);
    }

    if (isPong) {
        receive_concrete_message(inputMessage, inputMessageSize, inputPipe);
        printf("process[%i] received: %s\n", pid, inputMessage);
    }


    printf("%s process[%i] finished\n", outputMessage, pid);
}


void run_ping(pid_t pid, int maxReceivedCount, int inputPipe, int outputPipe) {
    run_ping_pong(pid, maxReceivedCount, inputPipe, outputPipe, "pong", "ping");
}

void run_pong(pid_t pid, int maxReceivedCount, int inputPipe, int outputPipe) {
    run_ping_pong(pid, maxReceivedCount, inputPipe, outputPipe, "ping", "pong");
}


int main() {
    int maxReplyCount = 20;

    int pingToPongWrite;
    int pingToPongRead;
    int pongToPingWrite;
    int pongToPingRead;


    create_pipe(&pingToPongRead, &pingToPongWrite);
    create_pipe(&pongToPingRead, &pongToPingWrite);

    int pid;

    if ((pid=fork()) == 0) {
        run_ping(pid, maxReplyCount, pongToPingRead, pingToPongWrite);
    } else {
        run_pong(pid, maxReplyCount, pingToPongRead, pongToPingWrite);
    }

    return 0;
}

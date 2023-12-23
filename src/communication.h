#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <unistd.h>

void create_pipe(int* inputPipe, int* outputPipe);

void send_message(int outputPipe, const char* buffer, size_t size);

void receive_message(int inputPipe, char* buffer, size_t size);

#endif
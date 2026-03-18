#ifndef SAFE_STDIN_H
#define SAFE_STDIN_H

#include <stdio.h>

int readInt(const char* prompt);
void readString(char* buffer, int size, const char* prompt);
int StrToInt(int* num, char* input);
int safeInputString(char* buffer, int size);

#endif
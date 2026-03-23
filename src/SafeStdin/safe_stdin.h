/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: safe_stdin.h
* Description: Safe stdin header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SAFE_STDIN_H_
#define SAFE_STDIN_H_

int SafeStdin(char* buf, int maxLen);
int readInt(const char* prompt);
void readString(char* buffer, int size, const char* prompt);
int StrToInt(int* num, char* input);
int safeInputString(char* buffer, int size);

#endif

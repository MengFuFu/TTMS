#define _CRT_SECURE_NO_WARNINGS
#include "safe_stdin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define SAFE_INPUT_SUCCESS  0
#define SAFE_INPUT_EMPTY    -2
#define SAFE_INPUT_EOF      -3

static int safeInputStringInternal(char* buffer, int size) {
    char* newline = NULL;
    int c = 0;

    if (buffer == NULL || size <= 1) {
        printf("Error: Invalid buffer parameter!\n");
        return -1;
    }

    memset(buffer, 0, size);

    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) {
            clearerr(stdin);
            return SAFE_INPUT_EOF;
        }
        else {
            clearerr(stdin);
            printf("Read failed, please retry! \n");
            return -2;
        }
    }

    newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    else {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long, automatically truncated \n");
    }

    if (strlen(buffer) == 0) {
        printf("Input cannot be empty! Please re-enter! \n");
        return SAFE_INPUT_EMPTY;
    }

    return SAFE_INPUT_SUCCESS;
}

int readInt(const char* prompt) {
    int num = 0;
    char input[100] = { 0 };
    int ret;

    while (1) {
        printf("%s", prompt);
        ret = safeInputStringInternal(input, sizeof(input));
        if (ret == SAFE_INPUT_EOF) {
            printf("\nDetected end of input, program exiting.\n");
            exit(EXIT_SUCCESS);
        }
        if (ret == SAFE_INPUT_SUCCESS && StrToInt(&num, input) == 1) {
            return num;
        }
    }
}

float readFloat(const char* prompt) {
    float num = 0.0f;
    char input[100] = { 0 };
    int ret;
    char* end = NULL;

    while (1) {
        printf("%s", prompt);
        ret = safeInputStringInternal(input, sizeof(input));
        if (ret == SAFE_INPUT_EOF) {
            printf("\nDetected end of input, program exiting.\n");
            exit(EXIT_SUCCESS);
        }
        if (ret == SAFE_INPUT_SUCCESS) {
            errno = 0;
            num = strtof(input, &end);
            if (end != input && errno == 0) {
                return num;
            } else {
                printf("Input error! Please enter a valid number.\n");
            }
        }
    }
}

void readString(char* buffer, int size, const char* prompt) {
    int ret;

    if (buffer == NULL || size <= 1) {
        printf("[Critical Error] Buffer must be a char array with size > 1! \n");
        return;
    }

    while (1) {
        printf("%s", prompt);
        ret = safeInputStringInternal(buffer, size);
        if (ret == SAFE_INPUT_EOF) {
            printf("\nDetected end of input, program exiting.\n");
            exit(EXIT_SUCCESS);
        }
        if (ret == SAFE_INPUT_SUCCESS) {
            return;
        }
    }
}

int StrToInt(int* num, char* input) {
    char* end = NULL;
    long val = 0;

    if (num == NULL || input == NULL) {
        printf("Error: Parameter cannot be NULL!\n");
        return 0;
    }

    errno = 0;
    val = strtol(input, &end, 10);

    if (end == input) {
        printf("Input error! No digits entered!\n");
        return 0;
    }
    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) {
        printf("Input error! Value out of int range!\n");
        return 0;
    }

    while (*end != '\0') {
        if (!isspace((unsigned char)*end)) {
            printf("Input error! Non-whitespace characters after digits.\n");
            return 0;
        }
        end++;
    }

    *num = (int)val;
    return 1;
}

int safeInputString(char* buffer, int size) {
    return safeInputStringInternal(buffer, size);
}

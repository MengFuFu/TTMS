#include "safe_stdin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/* 内部错误码 */
#define SAFE_INPUT_SUCCESS  0   /* 正常读取 */
#define SAFE_INPUT_EMPTY    -2  /* 输入为空（含仅空白） */
#define SAFE_INPUT_EOF      -3  /* 遇到文件结束符 */

/* 安全读取字符串：增强版，可检测EOF */
static int safeInputStringInternal(char* buffer, int size) {
    char* newline = NULL;
    int c = 0;

    if (buffer == NULL || size <= 1) {
        printf("错误：缓冲区参数非法！\n");
        return -1;
    }

    memset(buffer, 0, size);

    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) {
            clearerr(stdin);
            return SAFE_INPUT_EOF;   /* 文件结束，不再重试 */
        }
        else {
            clearerr(stdin);
            printf("读取失败，请重试！\n");
            return -2;
        }
    }

    /* 处理换行符和过长输入 */
    newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    else {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("输入过长，已自动截断！\n");
    }

    /* 检查空输入（含仅空白符的字符串） */
    if (strlen(buffer) == 0) {
        printf("输入不能为空！请重新输入！\n");
        return SAFE_INPUT_EMPTY;
    }

    return SAFE_INPUT_SUCCESS;
}

/* 公开接口：读取整数，循环直到合法输入；遇到EOF则退出程序 */
int readInt(const char* prompt) {
    int num = 0;
    char input[100] = { 0 };
    int ret;

    while (1) {
        printf("%s", prompt);
        ret = safeInputStringInternal(input, sizeof(input));
        if (ret == SAFE_INPUT_EOF) {
            printf("\n检测到输入结束，程序退出。\n");
            exit(EXIT_SUCCESS);
        }
        if (ret == SAFE_INPUT_SUCCESS && StrToInt(&num, input) == 1) {
            return num;
        }
        /* 其他情况（SAFE_INPUT_EMPTY或转换失败）继续循环 */
    }
}

/* 公开接口：读取非空字符串，循环直到合法输入；遇到EOF则退出程序 */
void readString(char* buffer, int size, const char* prompt) {
    int ret;

    if (buffer == NULL || size <= 1) {
        printf("【严重错误】缓冲区必须是字符数组，且大小必须大于1！\n");
        return;
    }

    while (1) {
        printf("%s", prompt);
        ret = safeInputStringInternal(buffer, size);
        if (ret == SAFE_INPUT_EOF) {
            printf("\n检测到输入结束，程序退出。\n");
            exit(EXIT_SUCCESS);
        }
        if (ret == SAFE_INPUT_SUCCESS) {
            return;
        }
        /* 空输入继续循环 */
    }
}

/* 字符串转整数，带完整错误检查 */
int StrToInt(int* num, char* input) {
    char* end = NULL;
    long val = 0;

    if (num == NULL || input == NULL) {
        printf("错误：参数不能为空！\n");
        return 0;
    }

    /* 跳过前导空白（strtol会自动跳过，但为了统一错误提示，可先检查空白串） */
    errno = 0;
    val = strtol(input, &end, 10);

    /* 错误判断：无任何数字 / 溢出 / 超出int范围 */
    if (end == input) {
        printf("输入错误！未输入任何数字。\n");
        return 0;
    }
    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) {
        printf("输入错误！数值超出int范围（%d ~ %d）！\n", INT_MIN, INT_MAX);
        return 0;
    }

    /* 检查剩余字符是否仅为空白 */
    while (*end != '\0') {
        if (!isspace((unsigned char)*end)) {
            printf("输入错误！数字后不能包含非空白字符。\n");
            return 0;
        }
        end++;
    }

    *num = (int)val;
    return 1;
}

/* 原有的safeInputString保留，可重写为调用内部函数，但为避免破坏接口，我们保持其原功能 */
int safeInputString(char* buffer, int size) {
    return safeInputStringInternal(buffer, size);
}
#define _CRT_SECURE_NO_WARNINGS
/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_XX
* File name: Account_Persist.c
* Description : 账户用例持久化层
* Author:   XUPT
* Version:  v.1
* Date: 	2025年
*/

#include "Account_Persist.h"
#include "../Service/Account.h"
#include "EntityKey_Persist.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";       // 账户数据文件名
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";// 账户临时文件名
static const char ACCOUNT_KEY_NAME[] = "Account";            // 账户主键名

/*
标识符：TTMS_SCU_Account_Perst_Insert
函数功能：向文件中添加一个新账户数据
参数说明：data 为 account_t 类型指针，待添加的账户数据
返 回 值：1 表示成功，0 表示失败
*/
int Account_Perst_Insert(account_t* data) {
    assert(NULL != data);

    // 分配新主键
    long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1);
    if (key <= 0) {
        return 0;
    }
    data->id = key;

    FILE* fp = fopen(ACCOUNT_DATA_FILE, "ab");
    int rtn = 0;
    if (NULL == fp) {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    rtn = fwrite(data, sizeof(account_t), 1, fp);
    fclose(fp);
    return rtn;
}

/*
标识符：TTMS_SCU_Account_Perst_Update
函数功能：在文件中更新一个账户数据
参数说明：data 为 account_t 类型指针，待更新的账户数据
返 回 值：1 表示成功，0 表示失败
*/
int Account_Perst_Update(const account_t* data) {
    assert(NULL != data);

    FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb+");
    if (NULL == fp) {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    account_t buf;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&buf, sizeof(account_t), 1, fp)) {
            if (buf.id == data->id) {
                fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
                fwrite(data, sizeof(account_t), 1, fp);
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

/*
标识符：TTMS_SCU_Account_Perst_DeleteByID
函数功能：从文件中删除一个账户数据
参数说明：ID 为待删除账户的 ID
返 回 值：1 表示成功，0 表示失败
*/
int Account_Perst_DeleteByID(int ID) {
    if (rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE) < 0) {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    FILE* fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
    if (NULL == fpSour) {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    FILE* fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
    if (NULL == fpTarg) {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
        fclose(fpSour);
        return 0;
    }

    account_t buf;
    int found = 0;
    while (!feof(fpSour)) {
        if (fread(&buf, sizeof(account_t), 1, fpSour)) {
            if (ID == buf.id) {
                found = 1;
                continue;
            }
            fwrite(&buf, sizeof(account_t), 1, fpTarg);
        }
    }

    fclose(fpTarg);
    fclose(fpSour);
    remove(ACCOUNT_DATA_TEMP_FILE);
    return found;
}

/*
标识符：TTMS_SCU_Account_Perst_SelectByID
函数功能：根据 ID 载入一个账户数据
参数说明：ID 为账户 ID，buf 为存储载入数据的指针
返 回 值：1 表示成功，0 表示失败
*/
int Account_Perst_SelectByID(int ID, account_t* buf) {
    assert(NULL != buf);

    FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    account_t data;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&data, sizeof(account_t), 1, fp)) {
            if (ID == data.id) {
                *buf = data;
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

/*
标识符：TTMS_SCU_Account_Perst_SelectByName
函数功能：根据用户名载入一个账户数据
参数说明：username 为用户名，buf 为存储载入数据的指针
返 回 值：1 表示成功，0 表示失败
*/
int Account_Perst_SelectByName(const char* username, account_t* buf) {
    assert(NULL != username && NULL != buf);

    FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    account_t data;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&data, sizeof(account_t), 1, fp)) {
            if (strcmp(data.username, username) == 0) {
                *buf = data;
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

/*
标识符：TTMS_SCU_Account_Perst_SelectAll
函数功能：载入所有账户数据到链表
参数说明：list 为 account_list_t 类型指针，指向账户链表头指针
返 回 值：成功载入的账户个数
*/
int Account_Perst_SelectAll(account_list_t list) {
    account_node_t* newNode;
    account_t data;
    int recCount = 0;

    assert(NULL != list);
    List_Free(list, account_node_t);

    FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(account_t), 1, fp)) {
            newNode = (account_node_t*)malloc(sizeof(account_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = data;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}
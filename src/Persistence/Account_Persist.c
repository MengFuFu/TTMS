#define _CRT_SECURE_NO_WARNINGS
/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Account_Persist.c
* Description: Account data persistence layer
* Author:   XUPT
* Version:  v.1
* Date:     2025
*/

#include "Account_Persist.h"
#include "../Service/Account.h"
#include "EntityKey_Persist.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";
static const char ACCOUNT_KEY_NAME[] = "Account";

int Account_Perst_Insert(account_t* data) {
    assert(NULL != data);

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
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}

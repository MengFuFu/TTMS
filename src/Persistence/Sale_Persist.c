/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Sale_Persist.c
* Description: Sale persistence implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Sale_Persist.h"
#include "EntityKey_Persist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char SALE_DATA_FILE[] = "Sale.dat";

// Identifier: TTMS_SCU_Sale_Perst_Insert
// Function: Insert a new sale record into file
// Input: data - sale data to insert
// Return: sale ID on success, 0 on failure
int Sale_Perst_Insert(const sale_t *data) {
    if (!data) return 0;

    int rtn = 0;
    FILE *fp = NULL;
    int id = EntKey_Perst_GetNewKeys("sale", 1);
    if (id <= 0) {
        return rtn;
    }

    fp = fopen(SALE_DATA_FILE, "ab");
    if (fp == NULL) {
        return rtn;
    }

    sale_t sale = *data;
    sale.id = id;
    if (fwrite(&sale, sizeof(sale_t), 1, fp) == 1) {
        rtn = id;
    }

    fclose(fp);
    return rtn;
}

// Identifier: TTMS_SCU_Sale_Perst_DeleteByID
// Function: Delete a sale record by ID from file
// Input: id - sale ID to delete
// Return: 1 on success, 0 on failure
int Sale_Perst_DeleteByID(int id) {
    if (id <= 0) return 0;

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    FILE *tmp = fopen("sale_tmp.dat", "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    sale_t buf;
    int found = 0;
    while (fread(&buf, sizeof(sale_t), 1, fp) == 1) {
        if (buf.id != id) {
            fwrite(&buf, sizeof(sale_t), 1, tmp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(SALE_DATA_FILE);
    rename("sale_tmp.dat", SALE_DATA_FILE);
    return found;
}

// Identifier: TTMS_SCU_Sale_Perst_SelectByID
// Function: Select a sale record by ID from file
// Input: id - sale ID, buf - buffer to store result
// Return: 1 on success, 0 on failure
int Sale_Perst_SelectByID(int id, sale_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(sale_t));

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    if (!fp) return 0;

    sale_t data;
    int found = 0;
    while (fread(&data, sizeof(sale_t), 1, fp) == 1) {
        if (data.id == id) {
            *buf = data;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

// Identifier: TTMS_SCU_Sale_Perst_SelectAll
// Function: Select all sale records into a list
// Input: list - sale list head pointer
// Return: number of records loaded
int Sale_Perst_SelectAll(sale_list_t list) {
    if (!list) return 0;

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    if (!fp) return 0;

    sale_t data;
    int count = 0;
    sale_node_t *newNode;

    while (fread(&data, sizeof(sale_t), 1, fp) == 1) {
        newNode = (sale_node_t *)malloc(sizeof(sale_node_t));
        if (newNode) {
            memset(newNode, 0, sizeof(sale_node_t));
            newNode->data = data;
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            count++;
        }
    }
    fclose(fp);
    return count;
}

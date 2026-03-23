/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Sale.c
* Description: Sale service implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Sale.h"
#include "../Persistence/Sale_Persist.h"
#include <stdlib.h>
#include <string.h>

// Identifier: TTMS_SCU_Sale_Srv_Add
// Function: Add a new sale record
// Input: data - sale data to add
// Return: sale ID on success, 0 on failure
int Sale_Srv_Add(const sale_t *data) {
    if (!data) return 0;
    return Sale_Perst_Insert(data);
}

// Identifier: TTMS_SCU_Sale_Srv_DeleteByID
// Function: Delete a sale record by ID
// Input: id - sale ID to delete
// Return: 1 on success, 0 on failure
int Sale_Srv_DeleteByID(int id) {
    if (id <= 0) return 0;
    return Sale_Perst_DeleteByID(id);
}

// Identifier: TTMS_SCU_Sale_Srv_FetchByID
// Function: Fetch a sale record by ID
// Input: id - sale ID, buf - buffer to store result
// Return: 1 on success, 0 on failure
int Sale_Srv_FetchByID(int id, sale_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(sale_t));
    return Sale_Perst_SelectByID(id, buf);
}

// Identifier: TTMS_SCU_Sale_Srv_FetchAll
// Function: Fetch all sale records into a list
// Input: list - sale list head pointer
// Return: number of records fetched
int Sale_Srv_FetchAll(sale_list_t list) {
    if (!list) return 0;
    return Sale_Perst_SelectAll(list);
}

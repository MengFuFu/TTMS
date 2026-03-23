/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Sale_Persist.h
* Description: Sale persistence header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SALE_PERSIST_H_
#define SALE_PERSIST_H_

#include "../Service/Sale.h"

// Identifier: TTMS_SCU_Sale_Perst_Insert
// Function: Insert a new sale record
int Sale_Perst_Insert(const sale_t *data);

// Identifier: TTMS_SCU_Sale_Perst_DeleteByID
// Function: Delete a sale by ID
int Sale_Perst_DeleteByID(int id);

// Identifier: TTMS_SCU_Sale_Perst_SelectByID
// Function: Select a sale by ID
int Sale_Perst_SelectByID(int id, sale_t *buf);

// Identifier: TTMS_SCU_Sale_Perst_SelectAll
// Function: Select all sales into a list
int Sale_Perst_SelectAll(sale_list_t list);

#endif

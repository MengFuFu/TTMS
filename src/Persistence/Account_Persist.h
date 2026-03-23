/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Account_Persist.h
* Description: Account persistence header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef ACCOUNT_PERSIST_H
#define ACCOUNT_PERSIST_H

#include "../Service/account.h"

// Identifier: TTMS_SCU_Account_Perst_Insert
// Function: Insert a new account record
int Account_Perst_Insert(account_t* data);

// Identifier: TTMS_SCU_Account_Perst_Update
// Function: Update an existing account record
int Account_Perst_Update(const account_t* data);

// Identifier: TTMS_SCU_Account_Perst_DeleteByID
// Function: Delete an account by ID
int Account_Perst_DeleteByID(int ID);

// Identifier: TTMS_SCU_Account_Perst_SelectByID
// Function: Select an account by ID
int Account_Perst_SelectByID(int ID, account_t* buf);

// Identifier: TTMS_SCU_Account_Perst_SelectByName
// Function: Select an account by username
int Account_Perst_SelectByName(const char* username, account_t* buf);

// Identifier: TTMS_SCU_Account_Perst_SelectAll
// Function: Select all accounts into a list
int Account_Perst_SelectAll(account_list_t list);

#endif

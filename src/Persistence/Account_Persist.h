#pragma once
#ifndef ACCOUNT_PERSIST_H
#define ACCOUNT_PERSIST_H

#include "../Service/Account.h"

int Account_Perst_Insert(account_t* data);
int Account_Perst_Update(const account_t* data);
int Account_Perst_DeleteByID(int ID);
int Account_Perst_SelectByID(int ID, account_t* buf);
int Account_Perst_SelectByName(const char* username, account_t* buf);
int Account_Perst_SelectAll(account_list_t list);

#endif
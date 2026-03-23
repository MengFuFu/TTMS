#define _CRT_SECURE_NO_WARNINGS
/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Account.c
* Description: Account business logic implementation
* Author:   XUPT
* Version:  v.1
* Date:     2025
*/

#include "Account.h"
#include "../Persistence/Account_Persist.h"
#include "../Common/list.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

account_t gl_CurUser;

int Account_Srv_Verify(char username[], char password[]) {
    account_t buf;
    if (Account_Perst_SelectByName(username, &buf) && strcmp(buf.password, password) == 0) {
        return 1;
    }
    return 0;
}

int Account_Srv_FetchByName(char username[], account_t* buf) {
    return Account_Perst_SelectByName(username, buf);
}

int Account_Srv_FetchAll(account_list_t list) {
    return Account_Perst_SelectAll(list);
}

account_node_t* Account_Srv_FindByUserName(account_list_t list, char username[]) {
    account_node_t* pNode;
    List_ForEach(list, pNode) {
        if (strcmp(pNode->data.username, username) == 0) {
            return pNode;
        }
    }
    return NULL;
}

account_node_t* Account_Srv_FindbyUserName(account_list_t list, char username[]) {
    return Account_Srv_FindByUserName(list, username);
}

int Account_Srv_Add(account_t* data) {
    return Account_Perst_Insert(data);
}

int Account_Srv_Modify(account_t* data) {
    return Account_Perst_Update(data);
}

int Account_Srv_DeleteByID(int ID) {
    return Account_Perst_DeleteByID(ID);
}

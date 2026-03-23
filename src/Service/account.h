#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "../Common/list.h"

typedef enum {
    USR_ANOMY = 0,
    USR_CLERK = 1,
    USR_MANG = 2,
    USR_ADMIN = 9
} user_type_t;

typedef struct {
    int id;
    char username[20];
    char password[20];
    user_type_t type;
} account_t;

typedef struct account_node {
    account_t data;
    struct account_node* prev, * next;
} account_node_t;

typedef account_node_t* account_list_t;

extern account_t gl_CurUser;

int Account_Srv_Verify(char username[], char password[]);
int Account_Srv_FetchByName(char username[], account_t* buf);
int Account_Srv_FetchAll(account_list_t list);
account_node_t* Account_Srv_FindByUserName(account_list_t list, char username[]);
account_node_t* Account_Srv_FindbyUserName(account_list_t list, char username[]);
int Account_Srv_Add(account_t* data);
int Account_Srv_Modify(account_t* data);
int Account_Srv_DeleteByID(int ID);

#endif

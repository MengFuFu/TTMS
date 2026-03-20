#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "../Common/list.h"

// 用户类型枚举
typedef enum {
    USR_ANOMY = 0,   // 匿名用户
    USR_CLERK = 1,   // 售票员
    USR_MANG = 2,    // 剧院经理
    USR_ADMIN = 9    // 系统管理员
} user_type_t;

// 账户结构体
typedef struct {
    int id;
    char username[20];
    char password[20];
    user_type_t type;
} account_t;

// 链表节点定义
typedef struct account_node {
    account_t data;
    struct account_node* prev, * next;
} account_node_t;

typedef account_node_t* account_list_t;

// 全局当前用户（你 UI 层里用到的 gl_CurUser）
extern account_t gl_CurUser;

// 服务层函数声明
int Account_Srv_Verify(char username[], char password[]);
int Account_Srv_FetchByName(char username[], account_t* buf);
int Account_Srv_FetchAll(account_list_t list);
account_node_t* Account_Srv_FindByUserName(account_list_t list, char username[]);
account_node_t* Account_Srv_FindbyUserName(account_list_t list, char username[]); // 兼容笔误
int Account_Srv_Add(account_t* data);
int Account_Srv_Modify(account_t* data);
int Account_Srv_DeleteByID(int ID);

#endif
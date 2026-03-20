#define _CRT_SECURE_NO_WARNINGS
/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_XX
* File name: Account.c
* Description : 账户用例服务层
* Author:   XUPT
* Version:  v.1
* Date: 	2025年
*/

#include "Account.h"
#include "../Persistence/Account_Persist.h"
#include "../Common/list.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

account_t gl_CurUser;

/*
函数功能：验证用户名和密码是否正确
参数说明：username 为用户名，password 为密码
返 回 值：1 表示验证通过，0 表示验证失败
*/
int Account_Srv_Verify(char username[], char password[]) {
    account_t buf;
    if (Account_Perst_SelectByName(username, &buf) && strcmp(buf.password, password) == 0) {
        return 1;
    }
    return 0;
}

/*
函数功能：根据用户名获取账户数据
参数说明：username 为用户名，buf 为存储账户数据的指针
返 回 值：1 表示成功，0 表示失败
*/
int Account_Srv_FetchByName(char username[], account_t* buf) {
    return Account_Perst_SelectByName(username, buf);
}

/*
函数功能：获取所有账户数据到链表
参数说明：list 为账户链表头指针
返 回 值：成功获取的账户个数
*/
int Account_Srv_FetchAll(account_list_t list) {
    return Account_Perst_SelectAll(list);
}

/*
函数功能：根据用户名查找账户节点
参数说明：list 为账户链表，username 为用户名
返 回 值：找到的账户节点指针，未找到返回 NULL
*/
account_node_t* Account_Srv_FindByUserName(account_list_t list, char username[]) {
    account_node_t* pNode;
    List_ForEach(list, pNode) {
        if (strcmp(pNode->data.username, username) == 0) {
            return pNode;
        }
    }
    return NULL;
}

// 修正你 UI 层里的笔误：FindbyUserName → FindByUserName
account_node_t* Account_Srv_FindbyUserName(account_list_t list, char username[]) {
    return Account_Srv_FindByUserName(list, username);
}

/*
函数功能：添加新账户
参数说明：data 为待添加的账户数据
返 回 值：1 表示成功，0 表示失败
*/
int Account_Srv_Add(account_t* data) {
    return Account_Perst_Insert(data);
}

/*
函数功能：修改账户信息
参数说明：data 为待修改的账户数据
返 回 值：1 表示成功，0 表示失败
*/
int Account_Srv_Modify(account_t* data) {
    return Account_Perst_Update(data);
}

/*
函数功能：根据 ID 删除账户
参数说明：ID 为待删除账户的 ID
返 回 值：1 表示成功，0 表示失败
*/
int Account_Srv_DeleteByID(int ID) {
    return Account_Perst_DeleteByID(ID);
}
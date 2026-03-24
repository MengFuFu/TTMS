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
#include <stdio.h>

account_t gl_CurUser;

static const char LOGIN_STATUS_FILE[] = "login_status.dat";

// 保存登录状态到本地文件
int Account_Srv_SaveLoginStatus(account_t* user) {
    FILE* fp = fopen(LOGIN_STATUS_FILE, "wb");
    if (fp == NULL) {
        return 0;
    }
    
    int rtn = fwrite(user, sizeof(account_t), 1, fp);
    fclose(fp);
    return rtn;
}

// 从本地文件读取登录状态
int Account_Srv_LoadLoginStatus(account_t* user) {
    FILE* fp = fopen(LOGIN_STATUS_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }
    
    int rtn = fread(user, sizeof(account_t), 1, fp);
    fclose(fp);
    return rtn;
}

// 清除登录状态
void Account_Srv_ClearLoginStatus() {
    remove(LOGIN_STATUS_FILE);
}

// 检查用户是否具有指定角色的权限
int Account_Srv_CheckPermission(int requiredRole) {
    // 管理员拥有所有权限
    if (gl_CurUser.type == USR_ADMIN) {
        return 1;
    }
    
    // 经理可以执行管理员和经理的操作
    if (gl_CurUser.type == USR_MANG && (requiredRole == USR_MANG || requiredRole == USR_CLERK)) {
        return 1;
    }
    
    // 售票员只能执行售票员的操作
    if (gl_CurUser.type == USR_CLERK && requiredRole == USR_CLERK) {
        return 1;
    }
    
    return 0;
}

// 检查当前用户是否为管理员
int Account_Srv_IsAdmin() {
    return gl_CurUser.type == USR_ADMIN;
}

// 检查当前用户是否为经理
int Account_Srv_IsManager() {
    return gl_CurUser.type == USR_MANG;
}

// 检查当前用户是否为售票员
int Account_Srv_IsClerk() {
    return gl_CurUser.type == USR_CLERK;
}

// 简单的XOR加密函数
void encryptPassword(char* password, char* encrypted) {
    const char key[] = "TTMS_PASSWORD_KEY";
    int keyLen = strlen(key);
    int passLen = strlen(password);
    
    for (int i = 0; i < passLen; i++) {
        encrypted[i] = password[i] ^ key[i % keyLen];
    }
    encrypted[passLen] = '\0';
}

// 验证密码
int verifyPassword(char* inputPassword, char* encryptedPassword) {
    char temp[20] = {0};
    encryptPassword(inputPassword, temp);
    return strcmp(temp, encryptedPassword) == 0;
}

int Account_Srv_Verify(char username[], char password[]) {
    account_t buf;
    if (Account_Perst_SelectByName(username, &buf)) {
        if (verifyPassword(password, buf.password)) {
            return 1;
        }
    }
    return 0;
}

int Account_Srv_Add(account_t* data) {
    // 加密密码
    char encryptedPassword[20] = {0};
    encryptPassword(data->password, encryptedPassword);
    strcpy(data->password, encryptedPassword);
    return Account_Perst_Insert(data);
}

int Account_Srv_Modify(account_t* data) {
    // 加密密码
    char encryptedPassword[20] = {0};
    encryptPassword(data->password, encryptedPassword);
    strcpy(data->password, encryptedPassword);
    return Account_Perst_Update(data);
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

int Account_Srv_DeleteByID(int ID) {
    return Account_Perst_DeleteByID(ID);
}

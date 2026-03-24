#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"
#include "./Service/Account.h"

// 初始化默认管理员账户
void InitDefaultAdmin() {
    account_t admin;
    // 检查是否已存在管理员账户
    if (!Account_Srv_FetchByName("admin", &admin)) {
        // 创建默认管理员账户
        strcpy(admin.username, "admin");
        strcpy(admin.password, "123123123");
        admin.type = USR_ADMIN;
        admin.id = 0;
        
        if (Account_Srv_Add(&admin)) {
            printf("Default admin account created successfully!\n");
        } else {
            printf("Failed to create default admin account!\n");
        }
    }
}

int main(void) {
    // 初始化默认管理员账户
    InitDefaultAdmin();
    
    // 尝试自动登录
    if (!Account_Srv_LoadLoginStatus(&gl_CurUser)) {
        // 自动登录失败，显示登录界面
        if (!SysLogin()) {
            return EXIT_FAILURE;
        }
    }
    
    Main_Menu();
    return EXIT_SUCCESS;
}
#define _CRT_SECURE_NO_WARNINGS
#include "Account_UI.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include "../Persistence/EntityKey_Persist.h"
#include "../SafeStdin/safe_stdin.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <conio.h>

extern account_t gl_CurUser;

static void Safe_Flush_Stdin(void)
{
    while (_kbhit())
    {
        _getch();
    }
}

int SysRegister(void)
{
    account_t newAccount;
    char ch;
    int count = 0;
    char password[20] = { 0 };
    int type = 0;
    account_list_t list;
    
    List_Init(list, account_node_t);
    Account_Srv_FetchAll(list);
    
    printf("\t\t\t=====================================================================\n");
    printf("\n");
    printf("\t\t\t Register New Account\n");
    printf("\t\t\t===============================\n");
    
    readString(newAccount.username, sizeof(newAccount.username), "\t\t\t Input username:");
    
    if (Account_Srv_FindByUserName(list, newAccount.username) != NULL)
    {
        printf("\t\t\t Username already exists!\n");
        List_Destroy(list, account_node_t);
        return 0;
    }
    
    printf("\t\t\t Input password:");
    while (1)
    {
        ch = _getch();
        if (ch == '\r' || ch == '\n')
        {
            password[count] = '\0';
            printf("\n");
            break;
        }
        if (ch == '\b')
        {
            if (count > 0)
            {
                printf("\b \b");
                count--;
            }
        }
        else
        {
            if (count < 19)
            {
                password[count++] = ch;
                printf("*");
            }
        }
    }
    strcpy(newAccount.password, password);
    
    type = readInt("\t\t\t Input user type (1.Clerk 2.Manager):");
    
    if (type == 1)
    {
        newAccount.type = USR_CLERK;
    }
    else if (type == 2)
    {
        newAccount.type = USR_MANG;
    }
    else
    {
        printf("\t\t\t Invalid type!\n");
        List_Destroy(list, account_node_t);
        return 0;
    }
    
    if (Account_Srv_Add(&newAccount))
    {
        printf("\t\t\t Account created successfully!\n");
        List_Destroy(list, account_node_t);
        return 1;
    }
    
    printf("\t\t\t Failed to create account!\n");
    List_Destroy(list, account_node_t);
    return 0;
}

int SysLogin(void)
{
    // 尝试自动登录
    if (Account_Srv_LoadLoginStatus(&gl_CurUser))
    {
        printf("Auto login success! Welcome back, %s!\n", gl_CurUser.username);
        printf("Press any key to continue...");
        _getch();
        return 1;
    }
    
    char choice[10];
    int loginAttempts = 0;
    const int MAX_ATTEMPTS = 3;
    
    while (1)
    {
        system("cls");
        printf("\t\t\t=====================================================================\n");
        printf("\t\t\t              Theater Ticket Management System\n");
        printf("\t\t\t=====================================================================\n");
        printf("\t\t\t [L]ogin\n");
        printf("\t\t\t [R]egister\n");
        printf("\t\t\t [E]xit\n");
        printf("\t\t\t=====================================================================\n");
        
        readString(choice, sizeof(choice), "\t\t\t Input choice:");
        
        switch (choice[0])
        {
        case 'L':
        case 'l':
        {
            char ch;
            int count = 0;
            char username[20] = { 0 };
            char password[20] = { 0 };
            account_t buf;
            
            if (loginAttempts >= MAX_ATTEMPTS)
            {
                printf("\t\t\t Too many login attempts! Please try again later.\n");
                printf("\t\t\t Press any key to continue...");
                _getch();
                return 0;
            }
            
            printf("\t\t\t=====================================================================\n");
            printf("\n");
            printf("\t\t\t Please input username:");
            readString(username, sizeof(username), "");
            
            printf("\t\t\t Please input password:");
            while (1)
            {
                ch = _getch();
                if (ch == '\r' || ch == '\n')
                {
                    password[count] = '\0';
                    printf("\n");
                    break;
                }
                if (ch == '\b')
                {
                    if (count > 0)
                    {
                        printf("\b \b");
                        count--;
                    }
                }
                else
                {
                    if (count < 19)
                    {
                        password[count++] = ch;
                        printf("*");
                    }
                }
            }
            printf("\t\t\t=====================================================================\n");
            
            if (Account_Srv_Verify(username, password))
            {
                printf("Login success!\n");
                Account_Srv_FetchByName(username, &buf);
                strcpy(gl_CurUser.username, buf.username);
                strcpy(gl_CurUser.password, buf.password);
                gl_CurUser.id = buf.id;
                gl_CurUser.type = buf.type;
                
                // 保存登录状态
                Account_Srv_SaveLoginStatus(&gl_CurUser);
                
                printf("Press any key to continue...");
                _getch();
                return 1;
            }
            
            loginAttempts++;
            printf("Username or password error! Attempts left: %d\n", MAX_ATTEMPTS - loginAttempts);
            printf("Press any key to continue...");
            _getch();
            break;
        }
        case 'R':
        case 'r':
            SysRegister();
            printf("Press any key to return to login menu...");
            _getch();
            break;
        case 'E':
        case 'e':
            return 0;
        default:
            printf("Invalid choice!\n");
            printf("Press any key to continue...");
            _getch();
            break;
        }
    }
}

void Account_UI_MgtEntry(void)
{
    int i;
    char choice[10] = { 0 };
    char username[30] = { 0 };
    account_list_t head;
    account_node_t* pos;
    Pagination_t paging;

    List_Init(head, account_node_t);
    paging.offset = 0;
    paging.pageSize = ACCOUNT_PAGE_SIZE;
    paging.totalRecords = Account_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head, paging);

    do
    {
        system("cls");

        printf("\n=================================================\n");
        printf(" User Information\n");
        printf("%5s %18s %10s %10s \n", "ID", "User Type", "Name", "Password");
        printf("\n-------------------------------------------------\n");
        Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i)
        {
            printf("%5d %18d %10s %10s \n", pos->data.id, pos->data.type, pos->data.username, pos->data.password);
        }

        printf("-------------------Page %2d/%2d---------------------\n",
            Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("***************************************************\n");
        printf("[P]rev [N]ext [A]dd [D]elete [U]pdate [S]earch [R]eturn\n");
        printf("\n=====================================================\n");
        
        readString(choice, sizeof(choice), "\t\t\tInput choice: ");

        switch (choice[0])
        {
        case 'a':
        case 'A':
            if (Account_UI_Add(head))
            {
                paging.totalRecords = Account_Srv_FetchAll(head);
                Paging_Locate_LastPage(head, paging, account_node_t);
            }
            break;
        case 'd':
        case 'D':
            readString(username, sizeof(username), "\t\t\tInput username to delete:");
            if (Account_UI_Delete(head, username))
            {
                paging.totalRecords = Account_Srv_FetchAll(head);
                List_Paging(head, paging, account_node_t);
            }
            break;
        case 'u':
        case 'U':
            readString(username, sizeof(username), "\t\t\tInput username to modify:");
            if (Account_UI_Modify(head, username))
            {
                paging.totalRecords = Account_Srv_FetchAll(head);
                List_Paging(head, paging, account_node_t);
            }
            break;
        case 's':
        case 'S':
            readString(username, sizeof(username), "\t\t\tInput username to search:");
            if (Account_UI_Query(head, username))
            {
                paging.totalRecords = Account_Srv_FetchAll(head);
                List_Paging(head, paging, account_node_t);
            }
            break;
        case 'p':
        case 'P':
            if (1 < Pageing_CurPage(paging))
            {
                Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
            {
                Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
            }
            break;
        case 'r':
        case 'R':
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }

        printf("Press any key to continue...");
        Safe_Flush_Stdin();
        _getch();
    } while (choice[0] != 'r' && choice[0] != 'R');

    List_Destroy(head, account_node_t);
}

int Account_UI_Add(account_list_t list)
{
    account_t newAccount;
    char ch;
    int count = 0;
    char password[20] = { 0 };
    int type = 0;
    
    memset(&newAccount, 0, sizeof(account_t));

    readString(newAccount.username, sizeof(newAccount.username), "\t\t\tInput new username:");

    if (Account_Srv_FindByUserName(list, newAccount.username) != NULL)
    {
        printf("\t\t\tUsername already exists!\n");
        return 0;
    }

    printf("\t\t\tInput new password:");
    while (1)
    {
        ch = _getch();
        if (ch == '\r' || ch == '\n')
        {
            password[count] = '\0';
            printf("\n");
            break;
        }
        if (ch == '\b')
        {
            if (count > 0)
            {
                printf("\b \b");
                count--;
            }
        }
        else
        {
            if (count < 19)
            {
                password[count++] = ch;
                printf("*");
            }
        }
    }
    strcpy(newAccount.password, password);

    type = readInt("Input user type (1.Clerk 2.Manager 9.Admin):");

    if (type == 1)
    {
        newAccount.type = USR_CLERK;
    }
    else if (type == 2)
    {
        newAccount.type = USR_MANG;
    }
    else if (type == 9)
    {
        newAccount.type = USR_ADMIN;
    }
    else if (type == 0)
    {
        newAccount.type = USR_ANOMY;
    }
    else
    {
        printf("Invalid type!\n");
        return 0;
    }

    if (Account_Srv_Add(&newAccount))
    {
        // 添加到列表
        account_list_t pNew = (account_list_t)malloc(sizeof(account_node_t));
        if (pNew == NULL)
        {
            printf("Memory allocation failed!\n");
            return 0;
        }
        memset(pNew, 0, sizeof(account_node_t));
        pNew->data = newAccount;
        List_AddTail(list, pNew);
        printf("Account created successfully!\n");
        return 1;
    }
    printf("Failed to create account!\n");
    return 0;
}

int Account_UI_Modify(account_list_t list, char userName[])
{
    account_list_t p = Account_Srv_FindByUserName(list, userName);
    char pwd[20] = { 0 };
    if (p == NULL)
    {
        printf("\t\t\tAccount not found!\n");
        return 0;
    }

    printf("Current info: %s\n", p->data.username);
    readString(pwd, sizeof(pwd), "\t\t\tInput new password:");
    strcpy(p->data.password, pwd);

    if (Account_Srv_Modify(&p->data))
    {
        printf("\t\t\tModify success!\n");
        return 1;
    }
    printf("\t\t\tModify failed!\n");
    return 0;
}

int Account_UI_Delete(account_list_t list, char userName[])
{
    account_list_t p = Account_Srv_FindByUserName(list, userName);
    if (p == NULL)
    {
        printf("\t\t\tAccount not found!\n");
        return 0;
    }

    if (Account_Srv_DeleteByID(p->data.id))
    {
        List_DelNode(p);
        free(p);
        printf("Delete success!\n");
        return 1;
    }
    printf("Delete failed!\n");
    return 0;
}

int Account_UI_Query(account_list_t list, char userName[])
{
    account_list_t pTemp = Account_Srv_FindByUserName(list, userName);
    if (pTemp == NULL)
    {
        printf("Account not found!\n");
        return 0;
    }

    printf("\n++++++++++++++++++++++++++++++++++++++\n");
    printf("     User Information\n");
    printf("%5s %18s %10s %10s \n", "ID", "Type", "Name", "Password");
    printf("----------------------------------------\n");
    printf("%5d %18d %10s %10s \n",
        pTemp->data.id, pTemp->data.type, pTemp->data.username, pTemp->data.password);
    return 1;
}

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

int SysLogin(void)
{
    char ch;
    int count = 0;
    char username[20] = { 0 };
    char password[20] = { 0 };
    account_t buf;

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
        printf("Press any key to continue...");
        _getch();
        return 1;
    }
    printf("Username or password error!\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
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
    account_list_t pNew;
    char ch;
    int count = 0;
    char password[20] = { 0 };
    int type = 0;
    pNew = (account_list_t)malloc(sizeof(account_node_t));
    if (pNew == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }
    memset(pNew, 0, sizeof(account_node_t));

    readString(pNew->data.username, sizeof(pNew->data.username), "\t\t\tInput new username:");

    if (Account_Srv_FindByUserName(list, pNew->data.username) != NULL)
    {
        printf("\t\t\tUsername already exists!\n");
        free(pNew);
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
    strcpy(pNew->data.password, password);

    type = readInt("Input user type (1.Clerk 2.Manager 9.Admin):");

    if (type == 1)
    {
        pNew->data.type = USR_CLERK;
    }
    else if (type == 2)
    {
        pNew->data.type = USR_MANG;
    }
    else if (type == 9)
    {
        pNew->data.type = USR_ADMIN;
    }
    else if (type == 0)
    {
        pNew->data.type = USR_ANOMY;
    }
    else
    {
        printf("Invalid type!\n");
        free(pNew);
        return 0;
    }

    pNew->data.id = EntKey_Perst_GetNewKeys("Account", 1);
    Account_Srv_Add(&pNew->data);
    List_AddTail(list, pNew);
    printf("Account created successfully!\n");
    return 1;
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

    printf("Current info: %s %s\n", p->data.username, p->data.password);
    readString(pwd, sizeof(pwd), "\t\t\tInput new password:");
    strcpy(p->data.password, pwd);

    Account_Srv_Modify(&p->data);
    printf("\t\t\tModify success!\n");
    return 1;
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

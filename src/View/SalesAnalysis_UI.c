#define _CRT_SECURE_NO_WARNINGS
#include "SalesAnalysis_UI.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include "../Service/Account.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

extern account_t gl_CurUser;
static const int SALESANALYSIS_PAGE_SIZE = 5;

void SalesAnalysis_UI_BoxOffice() {
}

void SalesAnalysis_UI_StaSale(int userID, user_date_t stDate, user_date_t endDate) {
}

void SalesAnalysis_UI_MgtEn() {
    int i;
    char choice[10] = { 0 };
    salesanalysis_list_t head;
    salesanalysis_node_t* pos;
    Pagination_t paging;
    List_Init(head, salesanalysis_node_t);
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;
    paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
    SalesAnalysis_Srv_SortBySale(head);
    Paging_Locate_FirstPage(head, paging);
    do {
        system("cls");
        printf("\n=============================================================================================================\n");
        printf("                                             Box Office Ranking                                                        \n");
        printf("%5s %18s %18s %10s %10s %10s %10s %9s %9s\n", "PlayID", "PlayName", "Area", "Duration", "Tickets", "Sales", "Price", "StartDate", "EndDate");
        printf("---------------------------------------------------------------------------------------------------------------\n");
        for (i = 0, pos = (salesanalysis_node_t*)paging.curPos; pos != head && i < paging.pageSize; i++) {
            printf("%5d %18s %18s %10d %10ld %10ld %10d %04d-%02d-%02d %04d-%02d-%02d\n",
                pos->data.play_id, pos->data.name, pos->data.area, pos->data.duration,
                pos->data.totaltickets, pos->data.sales, pos->data.price,
                pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
                pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
            pos = pos->next;
        }
        printf("\t\t\t-------------------Total:%2d--------------------Page %2d/%2d-------------------------------------------\n",
            paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("\nOptions: [P]rev Page | [N]ext Page | [R]eturn\n");
        
        readString(choice, sizeof(choice), "Input choice: ");
        
        switch (choice[0]) {
        case 'p':
        case 'P':
            if (1 < Pageing_CurPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
            }
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');
    List_Destroy(head, salesanalysis_node_t);
}

void StaSales_UI_Self() {
    int id = gl_CurUser.id;
    user_date_t curdate, startdate, enddate;
    curdate = DateNow();
    startdate.year = curdate.year;
    startdate.month = curdate.month;
    startdate.day = 1;
    enddate.year = curdate.year;
    enddate.month = curdate.month;
    enddate.day = 31;
    int moneyByMySelf = 0;
    char choice[10] = { 0 };
    do {
        system("cls");
        printf("\n============================================================================================================\n");
        printf("1. Query today's sales statistics\n");
        printf("2. Query this month's sales statistics\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("***************************************************************************************************\n");
        printf("\n=============================================================================================================\n");
        
        readString(choice, sizeof(choice), "Input choice: ");
        
        switch (choice[0]) {
        case '1':
            moneyByMySelf = StaSales_Srv_CompSaleVal(id, startdate, enddate);
            printf("\t\t\tYour sales: %d\n", moneyByMySelf);
            break;
        case '2':
            break;
        }
        printf("Press any key to continue...");
        _getch();
    } while (choice[0] != '0');
}

void StaSales_UI_Clerk() {
    int id, money;
    char Username[33];
    user_date_t startdate, enddate;
    account_t user;
    
    readString(Username, sizeof(Username), "\t\t\tInput clerk username: ");
    
    if (Account_Srv_FetchByName(Username, &user) && user.type == USR_CLERK) {
        id = user.id;
        printf("\t\t\tInput start date (year month day):\n");
        startdate.year = readInt("  Year: ");
        startdate.month = readInt("  Month: ");
        startdate.day = readInt("  Day: ");
        printf("\t\t\tInput end date (year month day):\n");
        enddate.year = readInt("  Year: ");
        enddate.month = readInt("  Month: ");
        enddate.day = readInt("  Day: ");
        money = StaSales_Srv_CompSaleVal(id, startdate, enddate);
        if (money == -1) {
            printf("\t\t\tNo sales record yet...\n");
            return;
        }
    }
    else if (!Account_Srv_FetchByName(Username, &user)) {
        printf("\t\t\tUser not found...\n");
        return;
    }
    else if (user.type != USR_CLERK) {
        printf("\t\t\tUser is not a clerk...\n");
        return;
    }
    printf("\t\t\tClerk %s sales: %d\n", Username, money);
    return;
}

void StaSales_UI_MgtEnt(account_t CurUser) {
    strcpy(gl_CurUser.username, CurUser.username);
    strcpy(gl_CurUser.password, CurUser.password);
    gl_CurUser.id = CurUser.id;
    gl_CurUser.type = CurUser.type;
    printf("%s----%s\n", gl_CurUser.username, CurUser.username);
    _getch();
    char choice[10] = { 0 };
    do {
        system("cls");
        printf("\n=============================================================================================================\n");
        printf("1. Statistics clerk sales...\n");
        printf("2. Statistics own sales...\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("***************************************************************************************************\n");
        printf("\n=============================================================================================================\n");
        
        readString(choice, sizeof(choice), "Input choice: ");
        
        switch (choice[0]) {
        case '1':
            if (gl_CurUser.type == USR_MANG) {
                StaSales_UI_Clerk();
            }
            else {
                printf("\t\t\tNo permission...\n");
            }
            break;
        case '2':
            if (gl_CurUser.type == USR_CLERK) {
                StaSales_UI_Self();
            }
            else {
                printf("\t\t\tNo permission...\n");
            }
            break;
        }
        printf("Press any key to continue...");
        _getch();
    } while (choice[0] != 'r' && choice[0] != 'R');
}

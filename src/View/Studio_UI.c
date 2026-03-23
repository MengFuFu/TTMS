/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Studio_UI.c
* Description: Studio management UI implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/
#define _CRT_SECURE_NO_WARNINGS
#include "../View/Studio_UI.h"
#include "../View/Seat_UI.h"
#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../SafeStdin/safe_stdin.h"

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

void Studio_UI_MgtEntry(void) {
    int i, id;
    char choice[10];

    studio_list_t head;
    studio_node_t* pos;
    Pagination_t paging;

    List_Init(head, studio_node_t);
    paging.offset = 0;
    paging.pageSize = STUDIO_PAGE_SIZE;

    paging.totalRecords = Studio_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head, paging);

    do {
        printf("\n====================================================================================================\n");
        printf("******************************  Projection Room List  ******************************\n");
        printf("%5s  %-24s  %12s  %12s  %12s\n",
            "ID", "Room Name", "Rows Count", "Columns Count", "Seats Count");
        printf("----------------------------------------------------------------------------------------------------\n");

        Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i) {
            printf("%5d  %-24s  %12d  %12d  %12d\n",
                pos->data.id,
                pos->data.name,
                pos->data.rowsCount,
                pos->data.colsCount,
                pos->data.seatsCount);
        }

        printf("----------------------------------------------------------------------------------------------------\n");
        printf("------- Total Records:%2d  ---------------------------------------  Page %2d/%2d  -------\n",
            paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("****************************************************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]eat | [R]eturn\n");
        printf("====================================================================================================\n");

        readString(choice, sizeof(choice), "Your Choice:");
        if (strlen(choice) != 1) {
            continue;
        }
        switch (choice[0]) {
        case 'a':
        case 'A':
            if (Studio_UI_Add()) {
                paging.totalRecords = Studio_Srv_FetchAll(head);
                Paging_Locate_LastPage(head, paging, studio_node_t);
            }
            break;
        case 'd':
        case 'D':
            id = readInt("Input the ID:");
            if (Studio_UI_Delete(id)) {
                paging.totalRecords = Studio_Srv_FetchAll(head);
                List_Paging(head, paging, studio_node_t);
            }
            break;
        case 'u':
        case 'U':
            id = readInt("Input the ID:");
            if (Studio_UI_Modify(id)) {
                paging.totalRecords = Studio_Srv_FetchAll(head);
                List_Paging(head, paging, studio_node_t);
            }
            break;
        case 's':
        case 'S':
            id = readInt("Input the ID:");
            Seat_UI_MgtEntry(id);
            paging.totalRecords = Studio_Srv_FetchAll(head);
            List_Paging(head, paging, studio_node_t);
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
            }
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');
    List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
    studio_t rec;
    int newRecCount = 0;
    char choice[10];
    seat_list_t seatList;

    do {
        printf("\n=======================================================\n");
        printf("****************  Add New Projection Room  ****************\n");
        printf("-------------------------------------------------------\n");

        readString(rec.name, sizeof(rec.name), "Room Name:");
        rec.rowsCount = readInt("Row Count of Seats:");
        rec.colsCount = readInt("Column Count of Seats:");

        rec.seatsCount = rec.rowsCount * rec.colsCount;
        printf("Total Seats Count: %d\n", rec.seatsCount);
        printf("=======================================================\n");

        if (Studio_Srv_Add(&rec)) {
            newRecCount += 1;
            printf("The new room added successfully!\n");
            
            List_Init(seatList, seat_node_t);
            int initCount = Seat_Srv_RoomInit(seatList, rec.id, rec.rowsCount, rec.colsCount);
            printf("Initialized %d seats with status '#' (Good).\n", initCount);
            List_Destroy(seatList, seat_node_t);
        }
        else {
            printf("The new room added failed!\n");
        }
        printf("-------------------------------------------------------\n");

        readString(choice, sizeof(choice), "[A]dd more, [R]eturn:");
    } while (choice[0] == 'a' || choice[0] == 'A');

    return newRecCount;
}

int Studio_UI_Modify(int id) {
    studio_t rec;
    int rtn = 0;
    int newrow, newcolumn;
    seat_list_t list;
    int seatcount;
    char tempInput[10];

    if (!Studio_Srv_FetchByID(id, &rec)) {
        printf("The room does not exist!\nPress [Enter] key to return!\n");
        readString(tempInput, sizeof(tempInput), "");
        return 0;
    }

    printf("\n=======================================================\n");
    printf("****************  Update Projection Room  ****************\n");
    printf("-------------------------------------------------------\n");
    printf("Room ID:%d\n", rec.id);
    printf("Room Name[%s]:", rec.name);

    readString(rec.name, sizeof(rec.name), "");

    List_Init(list, seat_node_t);
    seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
    if (seatcount) {
        do {
            printf("Row Count of Seats should >= [%d]:", rec.rowsCount);
            newrow = readInt("");
            printf("Column Count of Seats should >= [%d]:", rec.colsCount);
            newcolumn = readInt("");
        } while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
        rec.rowsCount = newrow;
        rec.colsCount = newcolumn;
        rec.seatsCount = seatcount;
    }
    else {
        rec.rowsCount = readInt("Row Count of Seats:");
        rec.colsCount = readInt("Column Count of Seats:");
        rec.seatsCount = 0;
    }

    printf("-------------------------------------------------------\n");

    if (Studio_Srv_Modify(&rec)) {
        rtn = 1;
        printf("The room data updated successfully!\nPress [R / r] key to return!\n");
    }
    else {
        printf("The room data updated failed!\nPress [R / r] key to return!\n");
    }

    readString(tempInput, sizeof(tempInput), "");
    return rtn;
}

int Studio_UI_Delete(int id) {
    int rtn = 0;
    char tempInput[10];

    if (Studio_Srv_DeleteByID(id)) {
        if (Seat_Srv_DeleteAllByRoomID(id))
            printf("The seats of the room deleted successfully!\n");
        printf("The room deleted successfully!\nPress [R / r] key to return!\n");
        rtn = 1;
    }
    else {
        printf("The room does not exist!\nPress [R / r] key to return!\n");
    }

    readString(tempInput, sizeof(tempInput), "");
    return rtn;
}

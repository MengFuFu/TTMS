#define _CRT_SECURE_NO_WARNINGS
#include "Schedule_UI.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"

void Schedule_UI_MgtEntry(int play_id)
{
    play_t buf;
    if (Play_Srv_FetchByID(play_id, &buf) == 0) {
        printf("No such play!\n");
        return;
    }

    int i, id;
    char choice[10];

    schedule_list_t list;
    schedule_node_t* pos;
    Pagination_t paging;

    List_Init(list, schedule_node_t);
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;

    paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
    Paging_Locate_FirstPage(list, paging);

    do {
        printf("\n==================================================================\n");
        printf("********************** Projection Room List **********************\n");
        printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "Name", "Studio",
            "Starttime", "Endtime");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i) {
            printf("%5d  %18s  %18s\n", pos->data.id, pos->data.name, pos->data.studio);
            printf("%dyear %dmonth %dday %02d:%02d\n",
                pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
                pos->data.starttime.hour, pos->data.starttime.minute);
            printf("%dyear %dmonth %dday %02d:%02d\n",
                pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
                pos->data.endtime.hour, pos->data.endtime.minute);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
            paging.totalRecords, Pageing_CurPage(paging),
            Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [R]eturn\n");
        printf("==================================================================\n");
        
        readString(choice, sizeof(choice), "Your Choice: ");

        switch (choice[0]) {
        case 'a':
        case 'A':
            if (Schedule_UI_Add(play_id)) {
                paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                Paging_Locate_LastPage(list, paging, schedule_node_t);
            }
            break;
        case 'd':
        case 'D':
            id = readInt("Input the ID: ");
            if (Schedule_UI_Delete(id)) {
                paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                List_Paging(list, paging, schedule_node_t);
            }
            break;
        case 'u':
        case 'U':
            id = readInt("Input the ID: ");
            if (Schedule_UI_Modify(id)) {
                paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                List_Paging(list, paging, schedule_node_t);
            }
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
            }
            break;
        case 'r':
        case 'R':
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');

    List_Destroy(list, schedule_node_t);
}

int Schedule_UI_Add(int play_id)
{
    int newcount = 0;
    schedule_t new;
    char temp[100];

    printf("\n=======================================================\n");
    printf("****************  Add New Show Schedule  ****************\n");
    printf("-------------------------------------------------------\n");
    
    new.id = readInt("The new Schedule id: ");
    readString(new.name, sizeof(new.name), "The new Schedule name: ");
    readString(new.studio, sizeof(new.studio), "The new Schedule's studio room: ");
    
    printf("The new Schedule's start data (year month day): ");
    new.startdate.year = readInt("  Year: ");
    new.startdate.month = readInt("  Month: ");
    new.startdate.day = readInt("  Day: ");
    
    printf("The new Schedule's start time (hour minute): ");
    new.starttime.hour = readInt("  Hour: ");
    new.starttime.minute = readInt("  Minute: ");
    
    printf("The new Schedule's end data (year month day): ");
    new.enddate.year = readInt("  Year: ");
    new.enddate.month = readInt("  Month: ");
    new.enddate.day = readInt("  Day: ");
    
    printf("The new Schedule's end time (hour minute): ");
    new.endtime.hour = readInt("  Hour: ");
    new.endtime.minute = readInt("  Minute: ");

    if (Schedule_Srv_Add(&new)) {
        printf("New schedule added successfully!\n");
        newcount = 1;
    }
    else {
        printf("Failed to add new schedule.\n");
    }

    return newcount;
}

int Schedule_UI_Modify(int id)
{
    schedule_list_t list;
    schedule_node_t* pos;
    schedule_t rec;
    int found = 0;
    int rtn = 0;
    char temp[100];

    List_Init(list, schedule_node_t);

    if (Schedule_Srv_FetchAll(list) <= 0) {
        printf("No schedules in system!\n");
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    List_ForEach(list, pos) {
        if (pos->data.id == id) {
            found = 1;
            rec = pos->data;
            break;
        }
    }

    if (!found) {
        printf("The schedule does not exist!\nPress [Enter] to return!\n");
        readString(temp, sizeof(temp), "");
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    printf("\n=======================================================\n");
    printf("****************  Update Schedule  ****************\n");
    printf("Schedule ID: %d\n", rec.id);
    printf("Play Name: %s\n", rec.name);

    printf("Studio [%s]: ", rec.studio);
    readString(rec.studio, sizeof(rec.studio), "");

    printf("Start Date [%04d-%02d-%02d] (YYYY-MM-DD):\n", 
        rec.startdate.year, rec.startdate.month, rec.startdate.day);
    rec.startdate.year = readInt("  Year: ");
    rec.startdate.month = readInt("  Month: ");
    rec.startdate.day = readInt("  Day: ");
    
    printf("Start Time [%02d:%02d] (HH:MM):\n", 
        rec.starttime.hour, rec.starttime.minute);
    rec.starttime.hour = readInt("  Hour: ");
    rec.starttime.minute = readInt("  Minute: ");

    printf("End Date [%04d-%02d-%02d] (YYYY-MM-DD):\n",
        rec.enddate.year, rec.enddate.month, rec.enddate.day);
    rec.enddate.year = readInt("  Year: ");
    rec.enddate.month = readInt("  Month: ");
    rec.enddate.day = readInt("  Day: ");
    
    printf("End Time [%02d:%02d] (HH:MM):\n",
        rec.endtime.hour, rec.endtime.minute);
    rec.endtime.hour = readInt("  Hour: ");
    rec.endtime.minute = readInt("  Minute: ");

    printf("-------------------------------------------------------\n");

    if (Schedule_Srv_Modify(&rec)) {
        printf("The schedule data updated successfully!\nPress [Enter] key to return!\n");
        rtn = 1;
    }
    else {
        printf("The schedule data update failed!\nPress [Enter] key to return!\n");
        rtn = 0;
    }

    readString(temp, sizeof(temp), "");
    List_Destroy(list, schedule_node_t);
    return rtn;
}

int Schedule_UI_Delete(int id)
{
    int rtn = 0;
    if (Schedule_Srv_DeleteByID(id) != 1) {
        printf("Failed to delete schedule!\n");
        rtn = 0;
    }
    else {
        printf("Schedule deleted successfully!\n");
        rtn = 1;
    }
    return rtn;
}

void Schedule_UI_ListAll(void) {
    schedule_list_t list;
    schedule_node_t* pos;
    int i = 0;

    List_Init(list, schedule_node_t);

    if (Schedule_Srv_FetchAll(list) <= 0) {
        printf("No schedules in system!\n");
        List_Destroy(list, schedule_node_t);
        return;
    }

    printf("\n==================================================================\n");
    printf("********************** Schedule List **********************\n");
    printf("%5s  %20s  %12s  %20s\n", "ID", "Play Name", "Studio", "Start - End");
    printf("------------------------------------------------------------------\n");

    List_ForEach(list, pos) {
        printf("%5d  %20s  %12s  %04d-%02d-%02d %02d:%02d  -  %04d-%02d-%02d %02d:%02d\n",
            pos->data.id, pos->data.name, pos->data.studio,
            pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
            pos->data.starttime.hour, pos->data.starttime.minute,
            pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
            pos->data.endtime.hour, pos->data.endtime.minute);
        i++;
    }

    printf("------------------------------------------------------------------\n");
    printf("Total records: %d\n", i);
    printf("==================================================================\n");

    List_Destroy(list, schedule_node_t);
}

int Schedule_UI_Query(char* play_name)
{
    schedule_list_t list;
    schedule_node_t* pos;
    int cnt = 0;

    if (play_name == NULL || strlen(play_name) == 0) {
        return 0;
    }

    List_Init(list, schedule_node_t);

    if (Schedule_Srv_FetchAll(list) <= 0) {
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    printf("\n==================================================================\n");
    printf("********************** Schedule Query Result **********************\n");
    printf("%5s  %20s  %12s  %20s\n", "ID", "Play Name", "Studio", "Start - End");
    printf("------------------------------------------------------------------\n");

    List_ForEach(list, pos) {
        if (strstr(pos->data.name, play_name) != NULL) {
            printf("%5d  %20s  %12s  %04d-%02d-%02d %02d:%02d  -  %04d-%02d-%02d %02d:%02d\n",
                pos->data.id, pos->data.name, pos->data.studio,
                pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
                pos->data.starttime.hour, pos->data.starttime.minute,
                pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
                pos->data.endtime.hour, pos->data.endtime.minute);
            cnt++;
        }
    }

    printf("------------------------------------------------------------------\n");
    printf("Matched records: %d\n", cnt);
    printf("==================================================================\n");

    List_Destroy(list, schedule_node_t);
    return cnt;
}

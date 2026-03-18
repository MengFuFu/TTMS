#define _CRT_SECURE_NO_WARNINGS


#include "Play_UI.h"
#include <stdio.h>
#include <string.h>
#include "../Common/List.h"

#define PLAY_PAGE_SIZE 5

// 辅助：枚举转字符串
static void Play_UI_Type2Str(play_type_t type, char* buf) {
    switch (type) {
    case PLAY_TYPE_FILM:    strcpy(buf, "Film"); break;
    case PLAY_TYPE_OPEAR:   strcpy(buf, "Opear"); break;
    case PLAY_TYPE_CONCERT: strcpy(buf, "Concert"); break;
    default: strcpy(buf, "Unknown");
    }
}

static void Play_UI_Rate2Str(play_rating_t rate, char* buf) {
    switch (rate) {
    case PLAY_RATE_CHILD:   strcpy(buf, "Child"); break;
    case PLAY_RATE_TEENAGE: strcpy(buf, "Teen"); break;
    case PLAY_RATE_ADULT:   strcpy(buf, "Adult"); break;
    default: strcpy(buf, "Unknown");
    }
}

// 管理剧目主界面
void Play_UI_MgtEntry(void) {
    int i, id;
    char choice[10] = { 0 };

    play_list_t head;
    play_node_t* pos;
    Pagination_t paging;

    // 初始化链表（传2个参数）
    List_Init(head, play_node_t);
    paging.offset = 0;
    paging.pageSize = PLAY_PAGE_SIZE;
    paging.totalRecords = Play_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head, paging);

    do {
        printf("\n==========================================================================================================\n");
        printf("****************************************  Play List  ****************************************\n");
        printf("%5s  %-20s  %-8s  %-8s  %8s  %10s  %12s  %12s\n",
            "ID", "Name", "Type", "Rating", "Duration", "Price", "Start Date", "End Date");
        printf("----------------------------------------------------------------------------------------------------------\n");

        Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i) {
            char typeStr[10] = { 0 }, rateStr[10] = { 0 };
            Play_UI_Type2Str(pos->data.type, typeStr);
            Play_UI_Rate2Str(pos->data.rating, rateStr);

            printf("%5d  %-20s  %-8s  %-8s  %6dmin  %10d  %04d-%02d-%02d  %04d-%02d-%02d\n",
                pos->data.id, pos->data.name, typeStr, rateStr,
                pos->data.duration, pos->data.price,
                pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
                pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
        }

        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("Total:%2d  |  Page %2d/%2d\n",
            paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("==========================================================================================================\n");
        printf("[A]dd | [U]pdate | [D]elete | [Q]uery | [P]rev | [N]ext | [R]eturn\n");
        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 'a': case 'A':
            if (Play_UI_Add()) {
                paging.totalRecords = Play_Srv_FetchAll(head);
                Paging_Locate_LastPage(head, paging, play_node_t);
            }
            break;
        case 'u': case 'U':
            id = readInt("Input Play ID:");
            if (Play_UI_Modify(id)) {
                paging.totalRecords = Play_Srv_FetchAll(head);
                List_Paging(head, paging, play_node_t);
            }
            break;
        case 'd': case 'D':
            id = readInt("Input Play ID:");
            if (Play_UI_Delete(id)) {
                paging.totalRecords = Play_Srv_FetchAll(head);
                List_Paging(head, paging, play_node_t);
            }
            break;
        case 'q': case 'Q':
            Play_UI_Query(head);
            break;
        case 'p': case 'P':
            if (!Pageing_IsFirstPage(paging))
                Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
            break;
        case 'n': case 'N':
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');

    // ✅ 修正：传2个参数
    List_Destroy(head, play_node_t);
}

// 添加新剧目
int Play_UI_Add(void) {
    play_t rec;
    // ✅ 关键：初始化清0，避免调试模式下的 0xCCCCCCCC
    memset(&rec, 0, sizeof(play_t));
    int cnt = 0;
    char choice[10] = { 0 };

    do {
        printf("\n==================== Add New Play ====================\n");
        readString(rec.name, sizeof(rec.name), "Play Name:");
        rec.type = readInt("Type (1.Film 2.Opear 3.Concert):");
        readString(rec.area, sizeof(rec.area), "Area:");
        rec.rating = readInt("Rating (1.Child 2.Teen 3.Adult):");
        rec.duration = readInt("Duration (min):");
        printf("Start Date:\n");
        rec.start_date.year = readInt("Year:");
        rec.start_date.month = readInt("Month:");
        rec.start_date.day = readInt("Day:");
        printf("End Date:\n");
        rec.end_date.year = readInt("Year:");
        rec.end_date.month = readInt("Month:");
        rec.end_date.day = readInt("Day:");
        rec.price = readInt("Price:");

        if (Play_Srv_Add(&rec)) {
            cnt++;
            printf("Add success! New Play ID: %d\n", rec.id); // ✅ 打印确认ID
        }
        else {
            printf("Add failed!\n");
        }
        readString(choice, sizeof(choice), "[A]dd more | [R]eturn:");
    } while (choice[0] == 'a' || choice[0] == 'A');
    return cnt;
}

// 修改剧目
int Play_UI_Modify(int id) {
    play_t rec;
    int rtn = 0;
    char temp[10] = { 0 };

    if (!Play_Srv_FetchByID(id, &rec)) {
        printf("Play not found!\nPress Enter to return...");
        readString(temp, sizeof(temp), "");
        return 0;
    }

    printf("\n==================== Update Play ====================\n");
    printf("Play ID: %d\n", rec.id);
    printf("Name[%s]:", rec.name); readString(rec.name, sizeof(rec.name), "");
    rec.type = readInt("Type (1.Film 2.Opear 3.Concert):");
    printf("Area[%s]:", rec.area); readString(rec.area, sizeof(rec.area), "");
    rec.rating = readInt("Rating (1.Child 2.Teen 3.Adult):");
    rec.duration = readInt("Duration (min):");
    printf("Start Date:\n");
    rec.start_date.year = readInt("Year:");
    rec.start_date.month = readInt("Month:");
    rec.start_date.day = readInt("Day:");
    printf("End Date:\n");
    rec.end_date.year = readInt("Year:");
    rec.end_date.month = readInt("Month:");
    rec.end_date.day = readInt("Day:");
    rec.price = readInt("Price:");

    if (Play_Srv_Modify(&rec)) {
        rtn = 1;
        printf("Update success!\nPress Enter to return...");
    }
    else {
        printf("Update failed!\nPress Enter to return...");
    }
    readString(temp, sizeof(temp), "");
    return rtn;
}

// 删除剧目
int Play_UI_Delete(int id) {
    int rtn = 0;
    char temp[10] = { 0 };

    if (Play_Srv_DeleteByID(id)) {
        rtn = 1;
        printf("Delete success!\nPress Enter to return...");
    }
    else {
        printf("Play not found!\nPress Enter to return...");
    }
    readString(temp, sizeof(temp), "");
    return rtn;
}

// 查询剧目
void Play_UI_Query(play_list_t list) {
    char key[31] = { 0 };
    readString(key, sizeof(key), "Input play name to search:");

    printf("\n==========================================================================================================\n");
    printf("****************************************  Query Result  ****************************************\n");
    printf("%5s  %-20s  %-8s  %-8s  %8s  %10s  %12s  %12s\n",
        "ID", "Name", "Type", "Rating", "Duration", "Price", "Start Date", "End Date");
    printf("----------------------------------------------------------------------------------------------------------\n");

    play_node_t* pos;
    int found = 0;
    List_ForEach(list, pos) {
        if (pos && strstr(pos->data.name, key)) {
            char typeStr[10] = { 0 }, rateStr[10] = { 0 };
            Play_UI_Type2Str(pos->data.type, typeStr);
            Play_UI_Rate2Str(pos->data.rating, rateStr);

            printf("%5d  %-20s  %-8s  %-8s  %6dmin  %10d  %04d-%02d-%02d  %04d-%02d-%02d\n",
                pos->data.id, pos->data.name, typeStr, rateStr,
                pos->data.duration, pos->data.price,
                pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
                pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
            found = 1;
        }
    }
    if (!found) printf("No matching play found!\n");
    printf("==========================================================================================================\n");
    char temp[10] = { 0 };
    readString(temp, sizeof(temp), "Press Enter to return...");
}
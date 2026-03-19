/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02
* File name: Seat_UI.c
* Description : 设置座位用例界面层
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Seat_UI.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
表识符：TTMS_SCU_Seat_UI_S2C
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
char Seat_UI_Status2Char(seat_status_t status) {
    switch (status) {
    case SEAT_GOOD:    return '#';  // 好座位
    case SEAT_BROKEN:  return 'x';  // 坏座位
    case SEAT_NONE:    return ' ';  // 无座位
    default:           return '?';
    }
}

/*
标识符：TTMS_SCU_Seat_UI_C2S
函数功能：根据输入符号获取座位状态。
参数说明：statusChar为字符型，表示设置座位的输入符号。
返 回 值：seat_status_t类型，表示座位的状态。
*/
seat_status_t Seat_UI_Char2Status(char statusChar) {
    switch (statusChar) {
    case '#': return SEAT_GOOD;
    case 'x': return SEAT_BROKEN;
    case ' ': return SEAT_NONE;
    default:  return SEAT_NONE;
    }
}

// 辅助：打印座位矩阵
static void Seat_UI_PrintMatrix(seat_list_t list, int rows, int cols) {
    int r, c;
    seat_node_t* pos;

    printf("\n==================== Seat Matrix ====================\n");
    printf("   ");
    for (c = 1; c <= cols; c++) printf("%2d", c); // 列号
    printf("\n");

    for (r = 1; r <= rows; r++) {
        printf("%2d ", r); // 行号
        for (c = 1; c <= cols; c++) {
            // 查找对应座位
            char statusChar = ' ';
            List_ForEach(list, pos) {
                if (pos->data.roomID == pos->data.roomID &&
                    pos->data.row == r && pos->data.column == c) {
                    statusChar = Seat_UI_Status2Char(pos->data.status);
                    break;
                }
            }
            printf(" %c", statusChar);
        }
        printf("\n");
    }
    printf("======================================================\n");
    printf("Status: #=Good | x=Broken |  =None\n");
}

/*
标识符：TTMS_SCU_Seat_UI_MgtEnt
函数功能：界面层管理座位的入口函数，显示当前的座位数据，并提供座位数据添加、修改、删除功能操作的入口。
参数说明：roomID为整型，是需要设置座位的演出厅ID。
返 回 值：无。
*/
void Seat_UI_MgtEntry(int roomID) {
    char choice[10] = { 0 };
    int row, col;
    studio_t studio;

    // 获取演出厅信息（行数、列数）
    if (!Studio_Srv_FetchByID(roomID, &studio)) {
        printf("Studio not found!\nPress Enter to return...");
        char temp[10] = { 0 };
        readString(temp, sizeof(temp), "");
        return;
    }

    seat_list_t seatList;
    List_Init(seatList, seat_node_t);
    Seat_Srv_FetchByRoomID(seatList, roomID);

    do {
        Seat_UI_PrintMatrix(seatList, studio.rowsCount, studio.colsCount);
        printf("\n[A]dd Seat | [M]odify Seat | [D]elete Seat | [R]eturn\n");
        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 'a': case 'A':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Add(seatList, roomID, row, col)) {
                printf("Add success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Add failed!\n");
            }
            break;

        case 'm': case 'M':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Modify(seatList, row, col)) {
                printf("Modify success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Modify failed!\n");
            }
            break;

        case 'd': case 'D':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Delete(seatList, row, col)) {
                printf("Delete success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Delete failed!\n");
            }
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');

    List_Destroy(seatList, seat_node_t);
}

/*
识符：TTMS_SCU_Seat_UI_Add
函数功能：用于添加一个新的座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，
         第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {
    seat_t rec;
    memset(&rec, 0, sizeof(seat_t));
    char statusChar[10] = { 0 };

    rec.roomID = roomID;
    rec.row = row;
    rec.column = column;

    printf("Input Seat Status (#=Good, x=Broken,  =None):");
    readString(statusChar, sizeof(statusChar), "");
    rec.status = Seat_UI_Char2Status(statusChar[0]);

    return Seat_Srv_Add(&rec);
}

/*
标识符：TTMS_SCU_Seat_UI_Mod
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
    seat_node_t* pos;
    char statusChar[10] = { 0 };

    // 查找对应座位
    List_ForEach(list, pos) {
        if (pos->data.row == row && pos->data.column == column) {
            printf("Current Status: %c\n", Seat_UI_Status2Char(pos->data.status));
            printf("Input New Status (#=Good, x=Broken,  =None):");
            readString(statusChar, sizeof(statusChar), "");
            pos->data.status = Seat_UI_Char2Status(statusChar[0]);
            return Seat_Srv_Modify(&pos->data);
        }
    }

    printf("Seat not found!\n");
    return 0;
}

/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
    seat_node_t* pos;

    // 查找对应座位
    List_ForEach(list, pos) {
        if (pos->data.row == row && pos->data.column == column) {
            return Seat_Srv_DeleteByID(pos->data.id);
        }
    }

    printf("Seat not found!\n");
    return 0;
}
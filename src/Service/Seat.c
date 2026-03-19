  /*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02
* File name: Seat.c
* Description : 设置座位用例业务逻辑层
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../Common/List.h"
#include "Seat.h"
#include "../Persistence/Seat_Persist.h"

/*
函数功能：用于添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_Srv_Add(const seat_t* data) {
    if (!data) return 0;

    // 生成自增ID
    seat_list_t tempList;
    List_Init(tempList, seat_node_t);
    Seat_Perst_SelectAll(tempList);

    int maxID = 0;
    seat_node_t* pos;
    List_ForEach(tempList, pos) {
        if (pos && pos->data.id > maxID && pos->data.id != 0xCCCCCCCC) {
            maxID = pos->data.id;
        }
    }

    // 强制转换掉const，因为要修改id
    seat_t* newData = (seat_t*)data;
    newData->id = maxID + 1;

    List_Destroy(tempList, seat_node_t);
    return Seat_Perst_Insert(newData);
}

/*
函数功能：批量添加座位数据。
参数说明：list为seat_list_t类型指针，表示需要添加的批量座位数据形成的链表。
返 回 值：整型，表示是否成功添加了批量座位的标志。
*/
int Seat_Srv_AddBatch(seat_list_t list) {
    if (!list) return 0;

    int count = 0;
    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (Seat_Srv_Add(&pos->data)) {
            count++;
        }
    }
    return count;
}

/*
函数功能：用于修改一个座位数据。
参数说明：data为seat_t类型指针，表示需要修改的座位数据结点。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_Srv_Modify(const seat_t* data) {
    if (!data) return 0;
    return Seat_Perst_Update(data);
}

/*
函数功能：根据座位ID删除一个座位。
参数说明：ID为整型，表示需要删除的座位数据结点。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Srv_DeleteByID(int ID) {
    return Seat_Perst_DeleteByID(ID);
}

/*
函数功能：根据座位ID获取座位数据。
参数说明：第一个参数ID为整型，表示座位ID，第二个参数buf为seat_t指针，指向待获取的座位数据结点。
返 回 值：整型，表示是否成功获取了座位的标志。
*/
int Seat_Srv_FetchByID(int ID, seat_t* buf) {
    if (!buf) return 0;
    memset(buf, 0, sizeof(seat_t));

    seat_list_t list;
    List_Init(list, seat_node_t);
    Seat_Perst_SelectAll(list);

    int found = 0;
    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (pos && pos->data.id == ID) {
            *buf = pos->data;
            found = 1;
            break;
        }
    }

    List_Destroy(list, seat_node_t);
    return found;
}

/*
函数功能：根据演出厅ID删除所有座位。
参数说明：roomID为整型，表示需要删除所有座位的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅所有座位的标志。
*/
int Seat_Srv_DeleteAllByRoomID(int roomID) {
    seat_list_t list;
    List_Init(list, seat_node_t);
    Seat_Perst_SelectAll(list);

    int count = 0;
    seat_node_t* pos, * next;
    for (pos = list->next; pos != list; pos = next) {
        next = pos->next;
        if (pos->data.roomID == roomID) {
            if (Seat_Perst_DeleteByID(pos->data.id)) {
                count++;
            }
        }
    }

    List_Destroy(list, seat_node_t);
    return count;
}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID) {
    if (!list) return 0;
    List_Free(list, seat_node_t);

    seat_list_t allList;
    List_Init(allList, seat_node_t);
    Seat_Perst_SelectAll(allList);

    int count = 0;
    seat_node_t* pos, * newNode;
    List_ForEach(allList, pos) {
        if (pos->data.roomID == roomID) {
            newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
            if (newNode) {
                memset(newNode, 0, sizeof(seat_node_t));
                newNode->data = pos->data;
                newNode->next = list->next;
                newNode->prev = list;
                if (list->next) list->next->prev = newNode;
                list->next = newNode;
                count++;
            }
        }
    }

    List_Destroy(allList, seat_node_t);
    return count;
}

/*
函数功能：根据演出厅ID获得该演出厅的有效座位。
参数说明：第一个参数list为seat_list_t类型，表示获取到的有效座位链表头指针，第二个参数roomID为整型，表示需要提取有效座位的演出厅ID。
返 回 值：整型，表示演出厅的有效座位个数。
*/
int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID) {
    if (!list) return 0;
    List_Free(list, seat_node_t);

    seat_list_t allList;
    List_Init(allList, seat_node_t);
    Seat_Perst_SelectAll(allList);

    int count = 0;
    seat_node_t* pos, * newNode;
    List_ForEach(allList, pos) {
        if (pos->data.roomID == roomID && pos->data.status != SEAT_NONE) {
            newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
            if (newNode) {
                memset(newNode, 0, sizeof(seat_node_t));
                newNode->data = pos->data;
                newNode->next = list->next;
                newNode->prev = list;
                if (list->next) list->next->prev = newNode;
                list->next = newNode;
                count++;
            }
        }
    }

    List_Destroy(allList, seat_node_t);
    return count;
}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount) {
    if (!list) return 0;
    List_Free(list, seat_node_t);

    // 先删除旧座位
    Seat_Srv_DeleteAllByRoomID(roomID);

    int count = 0;
    seat_t rec;
    memset(&rec, 0, sizeof(seat_t));
    rec.roomID = roomID;
    rec.status = SEAT_GOOD;

    for (int r = 1; r <= rowsCount; r++) {
        for (int c = 1; c <= colsCount; c++) {
            rec.row = r;
            rec.column = c;
            if (Seat_Srv_Add(&rec)) {
                count++;
            }
        }
    }

    // 重新加载到链表
    Seat_Srv_FetchByRoomID(list, roomID);
    return count;
}

/*
函数功能：对座位链表list按座位行号、列号进行排序。
参数说明：list为seat_list_t类型，表示待排序座位链表头指针。
返 回 值：无。
*/
void Seat_Srv_SortSeatList(seat_list_t list) {
    if (!list || list->next == list) return;

    // 简单冒泡排序：按行优先，列次之
    int swapped;
    seat_node_t* pos;
    seat_t temp;

    do {
        swapped = 0;
        for (pos = list->next; pos->next != list; pos = pos->next) {
            if (pos->data.row > pos->next->data.row ||
                (pos->data.row == pos->next->data.row && pos->data.column > pos->next->data.column)) {
                temp = pos->data;
                pos->data = pos->next->data;
                pos->next->data = temp;
                swapped = 1;
            }
        }
    } while (swapped);
}

/*
函数功能：将一个座位结点加入到已排序的座位链表中。
参数说明：第一个参数list为seat_list_t类型，表示待插入结点的座位链表头指针，第二个参数node为seat_node_t指针，表示需要插入的座位数据结点。
返 回 值：无。
*/
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t* node) {
    if (!list || !node) return;

    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (pos->data.row > node->data.row ||
            (pos->data.row == node->data.row && pos->data.column > node->data.column)) {
            // 插入到pos前面
            node->next = pos;
            node->prev = pos->prev;
            pos->prev->next = node;
            pos->prev = node;
            return;
        }
    }

    // 插入到末尾
    node->next = list;
    node->prev = list->prev;
    list->prev->next = node;
    list->prev = node;
}

/*
函数功能：根据座位的行、列号获取座位数据。
参数说明：第一个参数list为seat_list_t类型，表示座位链表头指针，
         第二个参数row为整型，表示待获取座位的行号，第三个参数column为整型，表示待获取座位的列号。
返 回 值：为seat_node_t指针，表示获取到的座位数据。
*/
seat_node_t* Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
    if (!list) return NULL;

    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (pos->data.row == row && pos->data.column == column) {
            return pos;
        }
    }
    return NULL;
}

/*
函数功能：根据座位ID在链表中获取座位数据。
参数说明：第一个参数list为seat_list_t类型，指向座位数据链表，第二个参数ID为整型，表示座位ID。
返 回 值：seat_node_t类型，表示获取的座位数据。
*/
seat_node_t* Seat_Srv_FindByID(seat_list_t list, int ID) {
    if (!list) return NULL;

    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (pos->data.id == ID) {
            return pos;
        }
    }
    return NULL;
}
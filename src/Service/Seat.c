/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Seat.c
* Description: Seat business logic implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../Common/List.h"
#include "Seat.h"
#include "../Persistence/Seat_Persist.h"

int Seat_Srv_Add(const seat_t* data) {
    if (!data) return 0;

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

    seat_t* newData = (seat_t*)data;
    newData->id = maxID + 1;

    List_Destroy(tempList, seat_node_t);
    return Seat_Perst_Insert(newData);
}

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

int Seat_Srv_Modify(const seat_t* data) {
    if (!data) return 0;
    return Seat_Perst_Update(data);
}

int Seat_Srv_DeleteByID(int ID) {
    return Seat_Perst_DeleteByID(ID);
}

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

int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount) {
    if (!list) return 0;
    List_Free(list, seat_node_t);

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

    Seat_Srv_FetchByRoomID(list, roomID);
    return count;
}

void Seat_Srv_SortSeatList(seat_list_t list) {
    if (!list || list->next == list) return;

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

void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t* node) {
    if (!list || !node) return;

    seat_node_t* pos;
    List_ForEach(list, pos) {
        if (pos->data.row > node->data.row ||
            (pos->data.row == node->data.row && pos->data.column > node->data.column)) {
            node->next = pos;
            node->prev = pos->prev;
            pos->prev->next = node;
            pos->prev = node;
            return;
        }
    }

    node->next = list;
    node->prev = list->prev;
    list->prev->next = node;
    list->prev = node;
}

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

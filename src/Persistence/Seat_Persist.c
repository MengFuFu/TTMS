/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Seat_Persist.c
* Description: Seat persistence layer implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

int Seat_Perst_Insert(seat_t* data) {
    assert(NULL != data);

    FILE* fp = fopen(SEAT_DATA_FILE, "ab");
    if (!fp) return 0;

    int rtn = fwrite(data, sizeof(seat_t), 1, fp);
    fclose(fp);
    return rtn;
}

int Seat_Perst_InsertBatch(seat_list_t list) {
    seat_node_t* p;
    assert(NULL != list);

    int count = 0;
    List_ForEach(list, p) {
        if (Seat_Perst_Insert(&p->data)) {
            count++;
        }
    }
    return count;
}

int Seat_Perst_Update(const seat_t* seatdata) {
    assert(NULL != seatdata);

    FILE* fp = fopen(SEAT_DATA_FILE, "rb+");
    if (!fp) return 0;

    seat_t buf;
    int found = 0;
    while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
        if (buf.id == seatdata->id) {
            fseek(fp, -(long)sizeof(seat_t), SEEK_CUR);
            fwrite(seatdata, sizeof(seat_t), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

int Seat_Perst_DeleteByID(int ID) {
    FILE* fp = fopen(SEAT_DATA_FILE, "rb");
    FILE* tmp = fopen(SEAT_DATA_TEMP_FILE, "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    seat_t buf;
    int found = 0;
    while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
        if (buf.id != ID) {
            fwrite(&buf, sizeof(seat_t), 1, tmp);
        }
        else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(SEAT_DATA_FILE);
    rename(SEAT_DATA_TEMP_FILE, SEAT_DATA_FILE);
    return found;
}

int Seat_Perst_DeleteAllByRoomID(int roomID) {
    FILE* fp = fopen(SEAT_DATA_FILE, "rb");
    FILE* tmp = fopen(SEAT_DATA_TEMP_FILE, "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    seat_t buf;
    int found = 0;
    while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
        if (buf.roomID != roomID) {
            fwrite(&buf, sizeof(seat_t), 1, tmp);
        }
        else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(SEAT_DATA_FILE);
    rename(SEAT_DATA_TEMP_FILE, SEAT_DATA_FILE);
    return found;
}

int Seat_Perst_SelectByID(int ID, seat_t* buf) {
    assert(NULL != buf);
    memset(buf, 0, sizeof(seat_t));

    FILE* fp = fopen(SEAT_DATA_FILE, "rb");
    if (!fp) return 0;

    seat_t temp;
    int found = 0;
    while (fread(&temp, sizeof(seat_t), 1, fp) == 1) {
        if (temp.id == ID) {
            *buf = temp;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

int Seat_Perst_SelectAll(seat_list_t list) {
    assert(NULL != list);
    List_Free(list, seat_node_t);

    FILE* fp = fopen(SEAT_DATA_FILE, "rb");
    if (!fp) return 0;

    seat_t data;
    int count = 0;
    seat_node_t* newNode;
    while (fread(&data, sizeof(seat_t), 1, fp) == 1) {
        newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
        if (newNode) {
            memset(newNode, 0, sizeof(seat_node_t));
            newNode->data = data;
            newNode->next = list->next;
            newNode->prev = list;
            if (list->next) list->next->prev = newNode;
            list->next = newNode;
            count++;
        }
    }
    fclose(fp);
    return count;
}

int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
    assert(NULL != list);
    List_Free(list, seat_node_t);

    FILE* fp = fopen(SEAT_DATA_FILE, "rb");
    if (!fp) return 0;

    seat_t data;
    int count = 0;
    seat_node_t* newNode;
    while (fread(&data, sizeof(seat_t), 1, fp) == 1) {
        if (data.roomID == roomID) {
            newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
            if (newNode) {
                memset(newNode, 0, sizeof(seat_node_t));
                newNode->data = data;
                newNode->next = list->next;
                newNode->prev = list;
                if (list->next) list->next->prev = newNode;
                list->next = newNode;
                count++;
            }
        }
    }
    fclose(fp);
    return count;
}

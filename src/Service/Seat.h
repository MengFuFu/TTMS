/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Seat.h
* Description: Seat business logic header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SEAT_H_
#define SEAT_H_

typedef enum {
    SEAT_NONE = 0,
    SEAT_GOOD = 1,
    SEAT_BROKEN = 9
} seat_status_t;

typedef struct {
    int id;
    int roomID;
    int row;
    int column;
    seat_status_t status;
} seat_t;

typedef struct seat_node {
    seat_t data;
    struct seat_node *next, *prev;
} seat_node_t, *seat_list_t;

int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount);
void Seat_Srv_SortSeatList(seat_list_t list);
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node);
int Seat_Srv_Add(const seat_t *data);
int Seat_Srv_AddBatch(seat_list_t list);
int Seat_Srv_Modify(const seat_t *data);
int Seat_Srv_DeleteByID(int ID);
int Seat_Srv_DeleteAllByRoomID(int roomID);
int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID);
int Seat_Srv_FetchByID(int ID, seat_t *buf);
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID);
seat_node_t *Seat_Srv_FindByRowCol(seat_list_t list, int row, int column);
seat_node_t *Seat_Srv_FindByID(seat_list_t list, int seatID);

#endif

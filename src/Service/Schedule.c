/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Schedule.c
* Description: Schedule business logic implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include <stdlib.h>
#include <assert.h>

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id){
    int rtn = Schedule_Perst_SelectByPlay(list, play_id);
    return rtn;
}

int Schedule_Srv_Add(schedule_t* data)
{
    int rtn = Schedule_Perst_Insert(data);
    if (rtn) {
        // 自动生成票务
        // 解析影厅名称，获取影厅ID (这里简化处理，实际需要从影厅数据中查找)
        int roomID = 1; // 假设默认影厅ID为1
        
        // 获取影厅的所有可用座位
        seat_list_t seatList;
        List_Init(seatList, seat_node_t);
        int seatCount = Seat_Srv_FetchValidByRoomID(seatList, roomID);
        
        if (seatCount > 0) {
            seat_node_t* pos;
            List_ForEach(seatList, pos) {
                // 为每个座位生成一张票
                ticket_t ticket;
                ticket.schedule_id = data->id;
                ticket.seat_id = pos->data.id;
                ticket.price = 50; // 假设默认票价为50
                ticket.status = TICKET_AVL;
                ticket.date = data->startdate;
                ticket.time = data->starttime;
                
                Ticket_Srv_Add(&ticket);
            }
        }
        
        List_Destroy(seatList, seat_node_t);
    }
    return rtn;
}

int Schedule_Srv_Modify(const schedule_t* data)
{
    int rtn = Schedule_Perst_Update(data);
    return rtn;
}

int Schedule_Srv_DeleteByID(int id)
{
    int rtn = Schedule_Perst_DeleteByID(id);
    return rtn;
}

int Schedule_Srv_FetchAll(schedule_list_t list)
{
    return Schedule_Perst_SelectAll(list);
}

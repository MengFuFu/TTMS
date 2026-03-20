/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale.c
* Description : 售票业务逻辑层实现
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Sale.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <string.h>

int Sale_Srv_Add(const sale_t *data) {
    if (!data) return 0;
    return Sale_Perst_Insert(data);
}

int Sale_Srv_DeleteByID(int id) {
    if (id <= 0) return 0;
    return Sale_Perst_DeleteByID(id);
}

int Sale_Srv_FetchByID(int id, sale_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(sale_t));
    return Sale_Perst_SelectByID(id, buf);
}

int Sale_Srv_FetchAll(sale_list_t list) {
    if (!list) return 0;
    return Sale_Perst_SelectAll(list);
}

int Ticket_Srv_Modify(const ticket_t *data) {
    if (!data || data->id <= 0) return 0;
    return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchBySchID(int id, ticket_list_t list) {
    if (!list || id <= 0) return 0;
    return Ticket_Perst_SelectBySchID(id, list);
}

int Ticket_Srv_FetchAll(ticket_list_t list) {
    if (!list) return 0;
    return Ticket_Perst_SelectAll(list);
}

ticket_node_t *Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id) {
    if (!list || seat_id <= 0) return NULL;
    return Ticket_Perst_SelectBySeatID(list, seat_id);
}

int Ticket_Srv_FetchByID(int id, ticket_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(ticket_t));
    return Ticket_Perst_SelectByID(id, buf);
}

int Schedule_Srv_Add(schedule_t *data) {
    if (!data) return 0;

    schedule_list_t tempList;
    List_Init(tempList, schedule_node_t);
    Schedule_Perst_SelectAll(tempList);

    int maxID = 0;
    schedule_node_t *pos;
    List_ForEach(tempList, pos) {
        if (pos && pos->data.id > maxID && pos->data.id != 0xCCCCCCCC) {
            maxID = pos->data.id;
        }
    }
    data->id = maxID + 1;

    List_Destroy(tempList, schedule_node_t);
    return Schedule_Perst_Insert(data);
}

int Schedule_Srv_Modify(const schedule_t *data) {
    if (!data || data->id <= 0) return 0;
    return Schedule_Perst_Update(data);
}

int Schedule_Srv_DeleteByID(int id) {
    if (id <= 0) return 0;
    return Schedule_Perst_DeleteByID(id);
}

int Schedule_Srv_FetchByID(int id, schedule_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(schedule_t));
    return Schedule_Perst_SelectByID(id, buf);
}

int Schedule_Srv_FetchByPlayID(schedule_list_t list, int playID) {
    if (!list || playID <= 0) return 0;
    return Schedule_Perst_SelectByPlayID(list, playID);
}

int Schedule_Srv_FetchAll(schedule_list_t list) {
    if (!list) return 0;
    return Schedule_Perst_SelectAll(list);
}

void Play_Srv_FilterByName(schedule_list_t list, const char *name) {
    if (!list || !name) return;
}

/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale_Persist.h
* Description : 售票持久层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SALE_PERSIST_H_
#define SALE_PERSIST_H_

#include "../Service/Sale.h"

int Sale_Perst_Insert(const sale_t *data);
int Sale_Perst_DeleteByID(int id);
int Sale_Perst_SelectByID(int id, sale_t *buf);
int Sale_Perst_SelectAll(sale_list_t list);

int Ticket_Perst_SelectBySchID(int id, ticket_list_t list);
ticket_node_t* Ticket_Perst_SelectBySeatID(ticket_list_t list, int seat_id);

//int Schedule_Perst_Insert(const schedule_t *data);
//int Schedule_Perst_Update(const schedule_t *data);
//int Schedule_Perst_DeleteByID(int id);
//int Schedule_Perst_SelectByID(int id, schedule_t *buf);
//int Schedule_Perst_SelectByPlayID(schedule_list_t list, int playID);
//int Schedule_Perst_SelectAll(schedule_list_t list);

#endif

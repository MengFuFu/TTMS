/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Ticket_Persist.h
* Description : 票持久层头文件
* Author:   XUPT
* Version:  v.1
*/

#ifndef TICKET_PERSIST_H_
#define TICKET_PERSIST_H_

#include "../Service/Ticket.h"

int Ticket_Perst_Insert(ticket_t *data);
int Ticket_Perst_Update(const ticket_t *data);
int Ticket_Perst_DeleteByID(int ID);
int Ticket_Perst_SelectByID(int ID, ticket_t *buf);
int Ticket_Perst_SelectBySchID(int id, ticket_list_t list);
ticket_list_t Ticket_Perst_SelectBySeatID(ticket_list_t list, int seat_id);
int Ticket_Perst_SelectAll(ticket_list_t list);

#endif

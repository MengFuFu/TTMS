/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Ticket_Persist.h
* Description: Ticket persistence header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef TICKET_PERSIST_H_
#define TICKET_PERSIST_H_

#include "../Service/Ticket.h"

// Identifier: TTMS_SCU_Ticket_Perst_Insert
// Function: Insert a new ticket record
int Ticket_Perst_Insert(ticket_t *data);

// Identifier: TTMS_SCU_Ticket_Perst_Update
// Function: Update an existing ticket record
int Ticket_Perst_Update(const ticket_t *data);

// Identifier: TTMS_SCU_Ticket_Perst_DeleteByID
// Function: Delete a ticket by ID
int Ticket_Perst_DeleteByID(int ID);

// Identifier: TTMS_SCU_Ticket_Perst_SelectByID
// Function: Select a ticket by ID
int Ticket_Perst_SelectByID(int ID, ticket_t *buf);

// Identifier: TTMS_SCU_Ticket_Perst_SelectBySchID
// Function: Select tickets by schedule ID
int Ticket_Perst_SelectBySchID(int id, ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Perst_SelectAll
// Function: Select all tickets into a list
int Ticket_Perst_SelectAll(ticket_list_t list);

#endif

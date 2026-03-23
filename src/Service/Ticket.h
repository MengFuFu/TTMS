/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Ticket.h
* Description: Ticket service header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef TICKET_H_
#define TICKET_H_

#include "../Common/common.h"
#include "../Common/List.h"
#include "Seat.h"

typedef enum {
    TICKET_AVL = 0,
    TICKET_SOLD = 1,
    TICKET_RESV = 9
} ticket_status_t;

typedef struct {
    int id;
    int schedule_id;
    int seat_id;
    float price;
    ticket_status_t status;
    user_time_t time;
    user_date_t date;
} ticket_t;

typedef struct ticket_node {
    ticket_t data;
    struct ticket_node *next;
    struct ticket_node *prev;
} ticket_node_t, *ticket_list_t;

// Identifier: TTMS_SCU_Ticket_Srv_Add
// Function: Add a new ticket
int Ticket_Srv_Add(ticket_t* data);

// Identifier: TTMS_SCU_Ticket_Srv_Modify
// Function: Modify an existing ticket
int Ticket_Srv_Modify(const ticket_t* data);

// Identifier: TTMS_SCU_Ticket_Srv_DeleteByID
// Function: Delete a ticket by ID
int Ticket_Srv_DeleteByID(int ID);

// Identifier: TTMS_SCU_Ticket_Srv_FetchByID
// Function: Fetch a ticket by ID
int Ticket_Srv_FetchByID(int ID, ticket_t* buf);

// Identifier: TTMS_SCU_Ticket_Srv_FetchBySchID
// Function: Fetch tickets by schedule ID
int Ticket_Srv_FetchBySchID(int id, ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Srv_FetchAll
// Function: Fetch all tickets into a list
int Ticket_Srv_FetchAll(ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Srv_FindByID
// Function: Find ticket node by ID in list
ticket_node_t* Ticket_Srv_FindByID(ticket_list_t list, int ticketID);

// Identifier: TTMS_SCU_Ticket_Srv_SelBySchID
// Function: Select tickets by schedule ID
int Ticket_Srv_SelBySchID(int id, ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Srv_Update
// Function: Update ticket status
int Ticket_Srv_Update(const ticket_t* data);

// Identifier: TTMS_SCU_Ticket_Perst_SelectBySchID
// Function: Select tickets by schedule ID from persistence
int Ticket_Perst_SelectBySchID(int id, ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Perst_SelectAll
// Function: Select all tickets from persistence
int Ticket_Perst_SelectAll(ticket_list_t list);

// Identifier: TTMS_SCU_Ticket_Perst_Insert
// Function: Insert ticket into persistence
int Ticket_Perst_Insert(ticket_t* data);

// Identifier: TTMS_SCU_Ticket_Perst_Update
// Function: Update ticket in persistence
int Ticket_Perst_Update(const ticket_t* data);

// Identifier: TTMS_SCU_Ticket_Perst_DeleteByID
// Function: Delete ticket from persistence by ID
int Ticket_Perst_DeleteByID(int ID);

#endif

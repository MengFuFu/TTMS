/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Ticket.c
* Description: Ticket service implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Ticket.h"
#include "../Persistence/Ticket_Persist.h"
#include "Seat.h"
#include "../Persistence/Seat_Persist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Identifier: TTMS_SCU_Ticket_Srv_Add
// Function: Add a new ticket record
// Input: data - ticket data to add
// Return: 1 on success, 0 on failure
int Ticket_Srv_Add(ticket_t* data) {
    if (!data) return 0;

    ticket_list_t tempList;
    List_Init(tempList, ticket_node_t);
    Ticket_Perst_SelectAll(tempList);

    int maxID = 0;
    ticket_node_t* pos;
    List_ForEach(tempList, pos) {
        if (pos && pos->data.id > maxID && pos->data.id != 0xCCCCCCCC) {
            maxID = pos->data.id;
        }
    }
    data->id = maxID + 1;

    List_Destroy(tempList, ticket_node_t);
    return Ticket_Perst_Insert(data);
}

// Identifier: TTMS_SCU_Ticket_Srv_Modify
// Function: Update an existing ticket record
// Input: data - ticket data to update
// Return: 1 on success, 0 on failure
int Ticket_Srv_Modify(const ticket_t* data) {
    if (!data) return 0;
    return Ticket_Perst_Update(data);
}

// Identifier: TTMS_SCU_Ticket_Srv_DeleteByID
// Function: Delete a ticket record by ID
// Input: ID - ticket ID to delete
// Return: 1 on success, 0 on failure
int Ticket_Srv_DeleteByID(int ID) {
    return Ticket_Perst_DeleteByID(ID);
}

// Identifier: TTMS_SCU_Ticket_Srv_FetchByID
// Function: Fetch a ticket record by ID
// Input: ID - ticket ID, buf - buffer to store result
// Return: 1 on success, 0 on failure
int Ticket_Srv_FetchByID(int ID, ticket_t* buf) {
    if (!buf) return 0;

    ticket_list_t list;
    List_Init(list, ticket_node_t);
    Ticket_Perst_SelectAll(list);

    int found = 0;
    ticket_node_t* pos;
    List_ForEach(list, pos) {
        if (pos && pos->data.id == ID) {
            *buf = pos->data;
            found = 1;
            break;
        }
    }

    List_Destroy(list, ticket_node_t);
    return found;
}

// Identifier: TTMS_SCU_Ticket_Srv_FetchBySchID
// Function: Fetch tickets by schedule ID into a list
// Input: id - schedule ID, list - ticket list head
// Return: number of tickets fetched
int Ticket_Srv_FetchBySchID(int id, ticket_list_t list) {
    if (!list) return 0;
    List_Free(list, ticket_node_t);
    return Ticket_Perst_SelectBySchID(id, list);
}

// Identifier: TTMS_SCU_Ticket_Srv_FetchAll
// Function: Fetch all ticket records into a list
// Input: list - ticket list head pointer
// Return: number of records fetched
int Ticket_Srv_FetchAll(ticket_list_t list) {
    if (!list) return 0;
    List_Free(list, ticket_node_t);
    return Ticket_Perst_SelectAll(list);
}

// Identifier: TTMS_SCU_Ticket_Srv_FindByID
// Function: Find ticket node by ID in list
// Input: list - ticket list head, ticketID - ticket ID
// Return: pointer to found node, NULL if not found
ticket_node_t* Ticket_Srv_FindByID(ticket_list_t list, int ticketID) {
    if (!list) return NULL;

    ticket_node_t* pos;
    List_ForEach(list, pos) {
        if (pos->data.id == ticketID) {
            return pos;
        }
    }
    return NULL;
}

// Identifier: TTMS_SCU_Ticket_Srv_SelBySchID
// Function: Select tickets by schedule ID
// Input: id - schedule ID, list - ticket list head
// Return: number of tickets selected
int Ticket_Srv_SelBySchID(int id, ticket_list_t list) {
    return Ticket_Srv_FetchBySchID(id, list);
}

// Identifier: TTMS_SCU_Ticket_Srv_Update
// Function: Update ticket record
// Input: data - ticket data to update
// Return: 1 on success, 0 on failure
int Ticket_Srv_Update(const ticket_t* data) {
    return Ticket_Srv_Modify(data);
}

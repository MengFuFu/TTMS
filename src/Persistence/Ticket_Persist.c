/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Ticket_Persist.c
* Description: Ticket persistence implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Ticket_Persist.h"
#include "EntityKey_Persist.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static const char TICKET_DATA_FILE[] = "Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat";
static const char TICKET_KEY_NAME[] = "Ticket";

// Identifier: TTMS_SCU_Ticket_Perst_Insert
// Function: Insert a new ticket record into file
// Input: data - ticket data to insert
// Return: 1 on success, 0 on failure
int Ticket_Perst_Insert(ticket_t* pData) {
    assert(NULL != pData);

    long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, 1);
    if (key <= 0)
        return 0;

    pData->id = (int)key;

    FILE* fp = fopen(TICKET_DATA_FILE, "ab");
    int rtn = 0;
    if (NULL == fp) {
        printf("Cannot open file %s!\n", TICKET_DATA_FILE);
        return 0;
    }

    rtn = fwrite(pData, sizeof(ticket_t), 1, fp);

    fclose(fp);
    return rtn;
}

// Identifier: TTMS_SCU_Ticket_Perst_Update
// Function: Update an existing ticket record in file
// Input: data - ticket data to update
// Return: 1 on success, 0 on failure
int Ticket_Perst_Update(const ticket_t* pData) {
    assert(NULL != pData);

    FILE* fp = fopen(TICKET_DATA_FILE, "rb+");
    if (NULL == fp) {
        printf("Cannot open file %s!\n", TICKET_DATA_FILE);
        return 0;
    }

    ticket_t buf;
    int found = 0;

    while (fread(&buf, sizeof(ticket_t), 1, fp) == 1)
    {
        if (buf.id == pData->id) {
            fseek(fp, -((long)sizeof(ticket_t)), SEEK_CUR);
            fwrite(pData, sizeof(ticket_t), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);

    return found;
}

// Identifier: TTMS_SCU_Ticket_Perst_DeleteByID
// Function: Delete a ticket record by ID from file
// Input: ID - ticket ID to delete
// Return: 1 on success, 0 on failure
int Ticket_Perst_DeleteByID(int ID) {
    if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0) {
        printf("Cannot open file %s!\n", TICKET_DATA_FILE);
        return 0;
    }

    FILE* fpSour, * fpTarg;
    fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
    if (NULL == fpSour) {
        printf("Cannot open file %s!\n", TICKET_DATA_FILE);
        return 0;
    }

    fpTarg = fopen(TICKET_DATA_FILE, "wb");
    if (NULL == fpTarg) {
        printf("Cannot open file %s!\n", TICKET_DATA_TEMP_FILE);
        fclose(fpSour);
        return 0;
    }

    ticket_t buf;
    int found = 0;

    while (fread(&buf, sizeof(ticket_t), 1, fpSour) == 1){
        if (ID == buf.id) {
            found = 1;
            continue;
        }
        fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
    }

    fclose(fpTarg);
    fclose(fpSour);

    remove(TICKET_DATA_TEMP_FILE);
    return found;
}

// Identifier: TTMS_SCU_Ticket_Perst_SelectByID
// Function: Select a ticket record by ID from file
// Input: ID - ticket ID, pBuf - buffer to store result
// Return: 1 on success, 0 on failure
int Ticket_Perst_SelectByID(int ID, ticket_t* pBuf) {
    assert(NULL != pBuf);

    FILE* fp = fopen(TICKET_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    ticket_t tmp;
    int found = 0;

    while (fread(&tmp, sizeof(ticket_t), 1, fp) == 1)
    {
        if (ID == tmp.id) {
            *pBuf = tmp;
            found = 1;
            break;
        }
    }
    fclose(fp);

    return found;
}

// Identifier: TTMS_SCU_Ticket_Perst_SelectAll
// Function: Select all ticket records into a list
// Input: list - ticket list head pointer
// Return: number of records loaded
int Ticket_Perst_SelectAll(ticket_list_t list) {
    ticket_node_t* newNode;
    ticket_t tmp;
    int recCount = 0;

    assert(NULL != list);

    List_Free(list, ticket_node_t);

    FILE* fp = fopen(TICKET_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (fread(&tmp, sizeof(ticket_t), 1, fp) == 1)
    {
        newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
        if (!newNode) {
            printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
            break;
        }
        newNode->data = tmp;
        newNode->next = newNode->prev = newNode;
        List_AddTail(list, newNode);
        recCount++;
    }
    fclose(fp);
    return recCount;
}

// Identifier: TTMS_SCU_Ticket_Perst_SelectBySchID
// Function: Select ticket records by schedule ID into a list
// Input: id - schedule ID, list - ticket list head
// Return: number of records loaded
int Ticket_Perst_SelectBySchID(int id, ticket_list_t list) {
    ticket_node_t* newNode;
    ticket_t tmp;
    int recCount = 0;

    assert(NULL != list);

    List_Free(list, ticket_node_t);

    FILE* fp = fopen(TICKET_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (fread(&tmp, sizeof(ticket_t), 1, fp) == 1)
    {
        if (tmp.schedule_id == id) {
            newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\n");
                break;
            }
            newNode->data = tmp;
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}

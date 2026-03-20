/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale_UI.c
* Description : 售票界面层实现
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Sale_UI.h"

#include "../Common/List.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static const int SALE_PAGE_SIZE = 5;

static void Sale_UI_GetCurrentTime(char *timeBuf, int bufSize) {
    if (!timeBuf || bufSize < 20) return;
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (t) {
        snprintf(timeBuf, bufSize, "%04d-%02d-%02d %02d:%02d",
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                 t->tm_hour, t->tm_min);
    }
}

static void Sale_UI_Status2Str(ticket_status_t status, char *buf) {
    if (!buf) return;
    switch (status) {
        case TICKET_AVL:     strcpy(buf, "Available"); break;
        case TICKET_SOLD:    strcpy(buf, "Sold"); break;
        case TICKET_RETURNED: strcpy(buf, "Returned"); break;
        case TICKET_RESV:    strcpy(buf, "Reserved"); break;
        default: strcpy(buf, "Unknown");
    }
}

static void Sale_UI_ScheduleStatus2Str(schedule_status_t status, char *buf) {
    if (!buf) return;
    switch (status) {
        case SCHEDULE_NOT_STARTED: strcpy(buf, "Not Started"); break;
        case SCHEDULE_ONGOING:    strcpy(buf, "Ongoing"); break;
        case SCHEDULE_ENDED:      strcpy(buf, "Ended"); break;
        case SCHEDULE_CANCELLED:  strcpy(buf, "Cancelled"); break;
        default: strcpy(buf, "Unknown");
    }
}

void Sale_UI_MgtEntry(void) {
    char choice[10] = {0};
    int playID;

    schedule_list_t head;
    Pagination_t paging;

    List_Init(head, schedule_node_t);
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = Schedule_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head, paging);

    do {
        printf("\n==================================================================\n");
        printf("********************** Ticket Sale Management **********************\n");
        printf("%5s  %10s  %18s  %18s  %8s  %12s\n", 
               "ID", "Play ID", "Start Time", "End Time", "Price", "Status");
        printf("------------------------------------------------------------------\n");

        int i;
        schedule_node_t *pos;
        Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i) {
            char statusStr[16] = {0};
            Sale_UI_ScheduleStatus2Str(pos->data.status, statusStr);
            printf("%5d  %10d  %18s  %18s  %8d  %12s\n",
                   pos->data.id, pos->data.play_id,
                   pos->data.start_time, pos->data.end_time,
                   pos->data.price, statusStr);
        }

        printf("------------------------------------------------------------------\n");
        printf("Total:%2d  |  Page %2d/%2d\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("==================================================================\n");
        printf("[C]hoose Play | [S]earch | [P]rev | [N]ext | [T]ickets | [R]efund | [E]xit\n");
        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 'c':
        case 'C':
            playID = readInt("Input play ID:");
            if (playID > 0) {
                Sale_UI_ShowScheduler(playID);
                paging.totalRecords = Schedule_Srv_FetchAll(head);
                List_Paging(head, paging, schedule_node_t);
            }
            break;
        case 's':
        case 'S': {
            char playName[50] = {0};
            readString(playName, sizeof(playName), "Input play name to search:");
            List_Destroy(head, schedule_node_t);
            List_Init(head, schedule_node_t);
            paging.offset = 0;
            paging.totalRecords = Schedule_Srv_FetchAll(head);
            Play_Srv_FilterByName(head, playName);
            break;
        }
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging))
                Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
            break;
        case 't':
        case 'T': {
            int scheduleID = readInt("Input schedule ID:");
            if (scheduleID > 0) {
                Sale_UI_ShowTicket(scheduleID);
            }
            break;
        }
        case 'r':
        case 'R':
            Sale_UI_RetfundTicket();
            break;
        }
    } while (choice[0] != 'e' && choice[0] != 'E');

    List_Destroy(head, schedule_node_t);
}

void Sale_UI_ShowScheduler(int playID) {
    if (playID <= 0) return;

    char choice[10] = {0};
    int scheduleID;

    schedule_list_t head;
    Pagination_t paging;

    List_Init(head, schedule_node_t);
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = Schedule_Srv_FetchByPlayID(head, playID);
    Paging_Locate_FirstPage(head, paging);

    do {
        printf("\n==================================================================\n");
        printf("********************** Schedule List **********************\n");
        printf("%5s  %18s  %18s  %8s  %12s\n", 
               "ID", "Start Time", "End Time", "Price", "Status");
        printf("------------------------------------------------------------------\n");

        int i;
        schedule_node_t *pos;
        Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i) {
            char statusStr[16] = {0};
            Sale_UI_ScheduleStatus2Str(pos->data.status, statusStr);
            printf("%5d  %18s  %18s  %8d  %12s\n",
                   pos->data.id, pos->data.start_time, 
                   pos->data.end_time, pos->data.price, statusStr);
        }

        printf("------------------------------------------------------------------\n");
        printf("Total:%2d  |  Page %2d/%2d\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("==================================================================\n");
        printf("[S]ell Ticket | [P]rev | [N]ext | [R]eturn\n");
        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 's':
        case 'S':
            scheduleID = readInt("Input schedule ID:");
            if (scheduleID > 0) {
                Sale_UI_SellTicket(scheduleID);
            }
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging))
                Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');

    List_Destroy(head, schedule_node_t);
}

void Sale_UI_ShowTicket(int scheduleID) {
    if (scheduleID <= 0) return;

    ticket_list_t head;
    Pagination_t paging;

    List_Init(head, ticket_node_t);
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = Ticket_Srv_FetchBySchID(scheduleID, head);
    Paging_Locate_FirstPage(head, paging);

    printf("\n==================================================================\n");
    printf("********************** Ticket List **********************\n");
    printf("%5s  %12s  %10s  %10s  %10s\n", 
           "ID", "Schedule ID", "Seat ID", "Price", "Status");
    printf("------------------------------------------------------------------\n");

    int i;
    ticket_node_t *pos;
    Paging_ViewPage_ForEach(head, paging, ticket_node_t, pos, i) {
        char statusStr[16] = {0};
        Sale_UI_Status2Str(pos->data.status, statusStr);
        printf("%5d  %12d  %10d  %10d  %10s\n",
               pos->data.id, pos->data.schedule_id,
               pos->data.seat_id, pos->data.price, statusStr);
    }

    printf("------------------------------------------------------------------\n");
    printf("Total:%2d  |  Page %2d/%2d\n",
           paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
    printf("==================================================================\n");

    List_Destroy(head, ticket_node_t);

    char temp[10] = {0};
    readString(temp, sizeof(temp), "Press Enter to return...");
}

int Sale_UI_SellTicket(int scheduleID) {
    if (scheduleID <= 0) return 0;

    int rtn = 0;
    int seatID;
    char choice[10] = {0};

    schedule_t schedule;
    if (!Schedule_Srv_FetchByID(scheduleID, &schedule)) {
        printf("Schedule not found!\n");
        char temp[10] = {0};
        readString(temp, sizeof(temp), "Press Enter to return...");
        return 0;
    }

    if (schedule.status != SCHEDULE_NOT_STARTED && schedule.status != SCHEDULE_ONGOING) {
        printf("This schedule is not available for ticket sale!\n");
        char temp[10] = {0};
        readString(temp, sizeof(temp), "Press Enter to return...");
        return 0;
    }

    ticket_list_t ticketList;
    List_Init(ticketList, ticket_node_t);
    Ticket_Srv_FetchBySchID(scheduleID, ticketList);

    do {
        printf("\n==================================================================\n");
        printf("********************** Sell Ticket **********************\n");
        printf("Schedule ID: %d\n", scheduleID);
        printf("Price: %d\n", schedule.price);
        printf("------------------------------------------------------------------\n");

        int i;
        ticket_node_t *pos;
        int availableCount = 0;
        
        printf("Available Tickets:\n");
        printf("%5s  %10s  %10s\n", "ID", "Seat ID", "Price");
        printf("------------------------------------------------------------------\n");
        
        List_ForEach(ticketList, pos) {
            if (pos && pos->data.status == TICKET_AVL) {
                printf("%5d  %10d  %10d\n",
                       pos->data.id, pos->data.seat_id, pos->data.price);
                availableCount++;
            }
        }
        
        if (availableCount == 0) {
            printf("No available tickets!\n");
            break;
        }
        
        printf("------------------------------------------------------------------\n");
        printf("Available: %d tickets\n", availableCount);

        seatID = readInt("Input seat ID to buy (0 to cancel):");
        if (seatID <= 0) {
            printf("Sale cancelled.\n");
            break;
        }

        ticket_node_t *ticketNode = Ticket_Srv_FetchBySeatID(ticketList, seatID);
        if (!ticketNode) {
            printf("Ticket not found for seat ID %d!\n", seatID);
            continue;
        }

        if (ticketNode->data.status != TICKET_AVL) {
            printf("This ticket is not available!\n");
            continue;
        }

        printf("Confirm to buy ticket for seat %d at price %d? (Y/N):", seatID, ticketNode->data.price);
        readString(choice, sizeof(choice), "");
        
        if (choice[0] == 'y' || choice[0] == 'Y') {
            ticketNode->data.status = TICKET_SOLD;
            if (Ticket_Srv_Modify(&ticketNode->data)) {
                sale_t sale;
                memset(&sale, 0, sizeof(sale_t));
                sale.ticket_id = ticketNode->data.id;
                sale.price = (float)ticketNode->data.price;
                Sale_UI_GetCurrentTime(sale.sale_time, sizeof(sale.sale_time));
                
                if (Sale_Srv_Add(&sale)) {
                    printf("Ticket sold successfully! Sale ID: %d\n", sale.id);
                    rtn = 1;
                } else {
                    ticketNode->data.status = TICKET_AVL;
                    Ticket_Srv_Modify(&ticketNode->data);
                    printf("Failed to create sale record!\n");
                }
            } else {
                printf("Failed to update ticket status!\n");
            }
        } else {
            printf("Sale cancelled.\n");
        }

        readString(choice, sizeof(choice), "Sell another ticket? (Y/N):");
    } while (choice[0] == 'y' || choice[0] == 'Y');

    List_Destroy(ticketList, ticket_node_t);
    return rtn;
}

int Sale_UI_RetfundTicket(void) {
    int rtn = 0;
    int ticketID;
    char choice[10] = {0};

    printf("\n==================================================================\n");
    printf("********************** Refund Ticket **********************\n");

    ticketID = readInt("Input ticket ID to refund:");
    if (ticketID <= 0) {
        printf("Invalid ticket ID!\n");
        char temp[10] = {0};
        readString(temp, sizeof(temp), "Press Enter to return...");
        return 0;
    }

    ticket_t ticket;
    memset(&ticket, 0, sizeof(ticket_t));
    
    if (!Ticket_Srv_FetchByID(ticketID, &ticket)) {
        printf("Ticket not found!\n");
        char temp[10] = {0};
        readString(temp, sizeof(temp), "Press Enter to return...");
        return 0;
    }

    if (ticket.status != TICKET_SOLD) {
        char statusStr[16] = {0};
        Sale_UI_Status2Str(ticket.status, statusStr);
        printf("This ticket cannot be refunded! Current status: %s\n", statusStr);
        char temp[10] = {0};
        readString(temp, sizeof(temp), "Press Enter to return...");
        return 0;
    }

    printf("\n==================================================================\n");
    printf("Ticket Information:\n");
    printf("------------------------------------------------------------------\n");
    printf("Ticket ID:    %d\n", ticket.id);
    printf("Schedule ID:  %d\n", ticket.schedule_id);
    printf("Seat ID:      %d\n", ticket.seat_id);
    printf("Price:        %d\n", ticket.price);
    printf("------------------------------------------------------------------\n");
    printf("Refund Amount: %d\n", ticket.price);
    printf("==================================================================\n");
    printf("Confirm to refund this ticket? (Y/N):");
    
    readString(choice, sizeof(choice), "");
    
    if (choice[0] == 'y' || choice[0] == 'Y') {
        ticket.status = TICKET_RETURNED;
        
        if (Ticket_Srv_Modify(&ticket)) {
            sale_t refundRecord;
            memset(&refundRecord, 0, sizeof(sale_t));
            refundRecord.ticket_id = ticket.id;
            refundRecord.price = -(float)ticket.price;
            Sale_UI_GetCurrentTime(refundRecord.sale_time, sizeof(refundRecord.sale_time));
            
            if (Sale_Srv_Add(&refundRecord)) {
                printf("\n==================================================================\n");
                printf("Refund successful!\n");
                printf("Refund Record ID: %d\n", refundRecord.id);
                printf("Refund Amount: %d\n", ticket.price);
                printf("==================================================================\n");
                rtn = 1;
            } else {
                ticket.status = TICKET_SOLD;
                Ticket_Srv_Modify(&ticket);
                printf("Failed to create refund record! Ticket status restored.\n");
            }
        } else {
            printf("Failed to update ticket status!\n");
        }
    } else {
        printf("Refund cancelled.\n");
    }

    char temp[10] = {0};
    readString(temp, sizeof(temp), "Press Enter to return...");
    return rtn;
}

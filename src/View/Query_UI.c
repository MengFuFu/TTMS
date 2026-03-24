#define _CRT_SECURE_NO_WARNINGS
#include "Query_UI.h"
#include "../Service/Play.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <string.h>

static void Query_UI_PlayInfo(void) {
    play_list_t playList;
    List_Init(playList, play_node_t);
    int playCount = Play_Srv_FetchAll(playList);
    
    if (playCount <= 0) {
        printf("No plays available!\n");
        char temp[10];
        readString(temp, sizeof(temp), "Press [R / r] to return...");
        List_Destroy(playList, play_node_t);
        return;
    }
    
    printf("\n==========================================================================================================\n");
    printf("****************************************  Play Information  ****************************************\n");
    printf("%5s  %-20s  %-8s  %-8s  %8s  %10s  %12s  %12s\n",
        "ID", "Name", "Type", "Rating", "Duration", "Price", "Start Date", "End Date");
    printf("----------------------------------------------------------------------------------------------------------\n");
    
    play_node_t* pos;
    List_ForEach(playList, pos) {
        char typeStr[10] = {0};
        char rateStr[10] = {0};
        
        switch (pos->data.type) {
        case PLAY_TYPE_FILM: strcpy(typeStr, "Film"); break;
        case PLAY_TYPE_OPEAR: strcpy(typeStr, "Opear"); break;
        case PLAY_TYPE_CONCERT: strcpy(typeStr, "Concert"); break;
        default: strcpy(typeStr, "Unknown");
        }
        
        switch (pos->data.rating) {
        case PLAY_RATE_CHILD: strcpy(rateStr, "Child"); break;
        case PLAY_RATE_TEENAGE: strcpy(rateStr, "Teen"); break;
        case PLAY_RATE_ADULT: strcpy(rateStr, "Adult"); break;
        default: strcpy(rateStr, "Unknown");
        }
        
        printf("%5d  %-20s  %-8s  %-8s  %6dmin  %10d  %04d-%02d-%02d  %04d-%02d-%02d\n",
            pos->data.id, pos->data.name, typeStr, rateStr,
            pos->data.duration, pos->data.price,
            pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
            pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
    }
    
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("Total plays: %d\n", playCount);
    printf("==========================================================================================================\n");
    
    char temp[10];
    readString(temp, sizeof(temp), "Press [R / r] to return...");
    List_Destroy(playList, play_node_t);
}

static void Query_UI_ScheduleInfo(void) {
    play_list_t playList;
    List_Init(playList, play_node_t);
    int playCount = Play_Srv_FetchAll(playList);
    
    if (playCount <= 0) {
        printf("No plays available!\n");
        char temp[10];
        readString(temp, sizeof(temp), "Press [R / r] to return...");
        List_Destroy(playList, play_node_t);
        return;
    }
    
    printf("\n==========================================================================================================\n");
    printf("****************************************  Play List  ****************************************\n");
    printf("%5s  %-20s\n", "ID", "Name");
    printf("----------------------------------------------------------------------------------------------------------\n");
    
    play_node_t* pos;
    List_ForEach(playList, pos) {
        printf("%5d  %-20s\n", pos->data.id, pos->data.name);
    }
    
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("Total plays: %d\n", playCount);
    printf("==========================================================================================================\n");
    
    int play_id = readInt("Input Play ID to query schedules: ");
    
    schedule_list_t scheduleList;
    List_Init(scheduleList, schedule_node_t);
    int scheduleCount = Schedule_Srv_FetchByPlay(scheduleList, play_id);
    
    if (scheduleCount <= 0) {
        printf("No schedules found for this play!\n");
    } else {
        printf("\n==========================================================================================================\n");
        printf("****************************************  Schedule Information  ****************************************\n");
        printf("%5s  %-20s  %-15s  %-19s  %-19s\n", "ID", "Name", "Studio", "Start Time", "End Time");
        printf("----------------------------------------------------------------------------------------------------------\n");
        
        schedule_node_t* sPos;
        List_ForEach(scheduleList, sPos) {
            printf("%5d  %-20s  %-15s  %04d-%02d-%02d %02d:%02d  %04d-%02d-%02d %02d:%02d\n",
                sPos->data.id, sPos->data.name, sPos->data.studio,
                sPos->data.startdate.year, sPos->data.startdate.month, sPos->data.startdate.day,
                sPos->data.starttime.hour, sPos->data.starttime.minute,
                sPos->data.enddate.year, sPos->data.enddate.month, sPos->data.enddate.day,
                sPos->data.endtime.hour, sPos->data.endtime.minute);
        }
        
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("Total schedules: %d\n", scheduleCount);
        printf("==========================================================================================================\n");
    }
    
    char temp[10];
    readString(temp, sizeof(temp), "Press [R / r] to return...");
    List_Destroy(playList, play_node_t);
    List_Destroy(scheduleList, schedule_node_t);
}

static void Query_UI_TicketInfo(void) {
    schedule_list_t scheduleList;
    List_Init(scheduleList, schedule_node_t);
    int scheduleCount = Schedule_Srv_FetchAll(scheduleList);
    
    if (scheduleCount <= 0) {
        printf("No schedules available!\n");
        char temp[10];
        readString(temp, sizeof(temp), "Press [R / r] to return...");
        List_Destroy(scheduleList, schedule_node_t);
        return;
    }
    
    printf("\n==========================================================================================================\n");
    printf("****************************************  Schedule List  ****************************************\n");
    printf("%5s  %-20s  %-15s\n", "ID", "Name", "Studio");
    printf("----------------------------------------------------------------------------------------------------------\n");
    
    schedule_node_t* pos;
    List_ForEach(scheduleList, pos) {
        printf("%5d  %-20s  %-15s\n", pos->data.id, pos->data.name, pos->data.studio);
    }
    
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("Total schedules: %d\n", scheduleCount);
    printf("==========================================================================================================\n");
    
    int schedule_id = readInt("Input Schedule ID to query tickets: ");
    
    ticket_list_t ticketList;
    List_Init(ticketList, ticket_node_t);
    int ticketCount = Ticket_Srv_FetchBySchID(schedule_id, ticketList);
    
    if (ticketCount <= 0) {
        printf("No tickets found for this schedule!\n");
    } else {
        printf("\n==========================================================================================================\n");
        printf("****************************************  Ticket Information  ****************************************\n");
        printf("%5s  %-10s  %-10s  %-10s  %-15s\n", "ID", "Seat ID", "Price", "Status", "Sale Time");
        printf("----------------------------------------------------------------------------------------------------------\n");
        
        ticket_node_t* tPos;
        List_ForEach(ticketList, tPos) {
            char statusStr[10] = {0};
            switch (tPos->data.status) {
            case TICKET_AVL: strcpy(statusStr, "Available"); break;
            case TICKET_SOLD: strcpy(statusStr, "Sold"); break;
            case TICKET_RESV: strcpy(statusStr, "Reserved"); break;
            default: strcpy(statusStr, "Unknown");
            }
            
            printf("%5d  %-10d  %-10.2f  %-10s  %04d-%02d-%02d %02d:%02d\n",
                tPos->data.id, tPos->data.seat_id, tPos->data.price, statusStr,
                tPos->data.date.year, tPos->data.date.month, tPos->data.date.day,
                tPos->data.time.hour, tPos->data.time.minute);
        }
        
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("Total tickets: %d\n", ticketCount);
        printf("==========================================================================================================\n");
    }
    
    char temp[10];
    readString(temp, sizeof(temp), "Press [R / r] to return...");
    List_Destroy(scheduleList, schedule_node_t);
    List_Destroy(ticketList, ticket_node_t);
}

static void Query_UI_SaleInfo(void) {
    sale_list_t saleList;
    List_Init(saleList, sale_node_t);
    int saleCount = Sale_Srv_FetchAll(saleList);
    
    if (saleCount <= 0) {
        printf("No sales records found!\n");
    } else {
        printf("\n==========================================================================================================\n");
        printf("****************************************  Sale Information  ****************************************\n");
        printf("%5s  %-10s  %-10s  %-20s\n", "ID", "Ticket ID", "Price", "Sale Time");
        printf("----------------------------------------------------------------------------------------------------------\n");
        
        sale_node_t* pos;
        List_ForEach(saleList, pos) {
            printf("%5d  %-10d  %-10d  %-20s\n",
                pos->data.id, pos->data.ticket_id, pos->data.price, pos->data.sale_time);
        }
        
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("Total sales: %d\n", saleCount);
        printf("==========================================================================================================\n");
    }
    
    char temp[10];
    readString(temp, sizeof(temp), "Press [R / r] to return...");
    List_Destroy(saleList, sale_node_t);
}

void Query_UI_MgtEntry(void) {
    char choice[10];
    do {
        system("cls");
        printf("\n==================================================================\n");
        printf("************************  Query Management  ************************\n");
        printf("[P]lay Information\n");
        printf("[S]chedule Information\n");
        printf("[T]icket Information\n");
        printf("[A]ll Sale Records\n");
        printf("[R]eturn\n");
        printf("==================================================================\n");
        readString(choice, sizeof(choice), "Please input your choice: ");
        
        switch (choice[0]) {
        case 'P': case 'p':
            Query_UI_PlayInfo();
            break;
        case 'S': case 's':
            Query_UI_ScheduleInfo();
            break;
        case 'T': case 't':
            Query_UI_TicketInfo();
            break;
        case 'A': case 'a':
            Query_UI_SaleInfo();
            break;
        case 'R': case 'r':
            break;
        default:
            printf("Invalid choice! Please try again.\n");
            break;
        }
    } while (choice[0] != 'R' && choice[0] != 'r');
}
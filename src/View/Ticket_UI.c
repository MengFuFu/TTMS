#define _CRT_SECURE_NO_WARNINGS
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Schedule.h"
#include "../Service/Seat.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <stdlib.h>

void Ticket_UI_Main(void) {
    while (1) {
        printf("\n==================== Ticket Management ====================\n");
        printf("1. Generate tickets for schedule\n");
        printf("2. Regenerate tickets for schedule\n");
        printf("3. Manage tickets (Add/Delete/Modify/Status)\n");
        printf("0. Return\n");
        printf("======================================================\n");

        int choice = readInt("Input choice: ");

        switch (choice) {
        case 1:
            Ticket_UI_GenTicket();
            break;
        case 2:
            Ticket_UI_ReGenTicket();
            break;
        case 3:
            Ticket_UI_ManageTickets();
            break;
        case 0:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void Ticket_UI_GenTicket(void) {
    printf("\n--------------------- Generate Tickets ---------------------\n");
    
    // 显示所有演出计划
    schedule_list_t list;
    List_Init(list, schedule_node_t);
    int count = Schedule_Srv_FetchAll(list);
    
    if (count <= 0) {
        printf("No schedules available!\n");
        List_Destroy(list, schedule_node_t);
        return;
    }
    
    printf("Available schedules:\n");
    printf("%-5s %-20s %-15s %-19s\n", "ID", "Name", "Studio", "Start Time");
    printf("-----------------------------------------------------------\n");
    
    schedule_node_t* pos;
    int i = 1;
    List_ForEach(list, pos) {
        printf("%-5d %-20s %-15s %04d-%02d-%02d %02d:%02d\n",
            pos->data.id, pos->data.name, pos->data.studio,
            pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
            pos->data.starttime.hour, pos->data.starttime.minute);
        i++;
    }
    
    // 让用户选择演出计划
    int scheduleId = readInt("Input schedule ID to generate tickets: ");
    
    // 验证演出计划是否存在
    schedule_node_t* selectedSchedule = NULL;
    List_ForEach(list, pos) {
        if (pos->data.id == scheduleId) {
            selectedSchedule = pos;
            break;
        }
    }
    
    if (!selectedSchedule) {
        printf("Invalid schedule ID!\n");
        List_Destroy(list, schedule_node_t);
        return;
    }
    
    // 解析影厅名称，获取影厅ID (这里简化处理，实际需要从影厅数据中查找)
    int roomID = 1; // 假设默认影厅ID为1
    
    // 获取影厅的所有可用座位
    seat_list_t seatList;
    List_Init(seatList, seat_node_t);
    int seatCount = Seat_Srv_FetchValidByRoomID(seatList, roomID);
    
    if (seatCount <= 0) {
        printf("No available seats in the studio!\n");
        List_Destroy(list, schedule_node_t);
        List_Destroy(seatList, seat_node_t);
        return;
    }
    
    // 为每个座位生成一张票
    int generatedCount = 0;
    seat_node_t* seatPos;
    List_ForEach(seatList, seatPos) {
        ticket_t ticket;
        ticket.schedule_id = scheduleId;
        ticket.seat_id = seatPos->data.id;
        ticket.price = 50; // 假设默认票价为50
        ticket.status = TICKET_AVL;
        ticket.date = selectedSchedule->data.startdate;
        ticket.time = selectedSchedule->data.starttime;
        
        if (Ticket_Srv_Add(&ticket)) {
            generatedCount++;
        }
    }
    
    printf("Successfully generated %d tickets for schedule ID %d!\n", generatedCount, scheduleId);
    printf("Press any key to continue...");
    _getch();
    
    List_Destroy(list, schedule_node_t);
    List_Destroy(seatList, seat_node_t);
}

void Ticket_UI_ReGenTicket(void) {
    printf("\n------------------- Regenerate Tickets -------------------\n");
    
    // 显示所有演出计划
    schedule_list_t list;
    List_Init(list, schedule_node_t);
    int count = Schedule_Srv_FetchAll(list);
    
    if (count <= 0) {
        printf("No schedules available!\n");
        List_Destroy(list, schedule_node_t);
        return;
    }
    
    printf("Available schedules:\n");
    printf("%-5s %-20s %-15s %-19s\n", "ID", "Name", "Studio", "Start Time");
    printf("-----------------------------------------------------------\n");
    
    schedule_node_t* pos;
    int i = 1;
    List_ForEach(list, pos) {
        printf("%-5d %-20s %-15s %04d-%02d-%02d %02d:%02d\n",
            pos->data.id, pos->data.name, pos->data.studio,
            pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
            pos->data.starttime.hour, pos->data.starttime.minute);
        i++;
    }
    
    // 让用户选择演出计划
    int scheduleId = readInt("Input schedule ID to regenerate tickets: ");
    
    // 验证演出计划是否存在
    schedule_node_t* selectedSchedule = NULL;
    List_ForEach(list, pos) {
        if (pos->data.id == scheduleId) {
            selectedSchedule = pos;
            break;
        }
    }
    
    if (!selectedSchedule) {
        printf("Invalid schedule ID!\n");
        List_Destroy(list, schedule_node_t);
        return;
    }
    
    // 删除该演出计划的所有现有票务
    ticket_list_t existingTickets;
    List_Init(existingTickets, ticket_node_t);
    int existingCount = Ticket_Srv_FetchBySchID(scheduleId, existingTickets);
    
    if (existingCount > 0) {
        ticket_node_t* ticketPos;
        List_ForEach(existingTickets, ticketPos) {
            Ticket_Srv_DeleteByID(ticketPos->data.id);
        }
        printf("Deleted %d existing tickets for schedule ID %d!\n", existingCount, scheduleId);
    }
    
    // 解析影厅名称，获取影厅ID (这里简化处理，实际需要从影厅数据中查找)
    int roomID = 1; // 假设默认影厅ID为1
    
    // 获取影厅的所有可用座位
    seat_list_t seatList;
    List_Init(seatList, seat_node_t);
    int seatCount = Seat_Srv_FetchValidByRoomID(seatList, roomID);
    
    if (seatCount <= 0) {
        printf("No available seats in the studio!\n");
        List_Destroy(list, schedule_node_t);
        List_Destroy(existingTickets, ticket_node_t);
        List_Destroy(seatList, seat_node_t);
        return;
    }
    
    // 为每个座位生成一张票
    int generatedCount = 0;
    seat_node_t* seatPos;
    List_ForEach(seatList, seatPos) {
        ticket_t ticket;
        ticket.schedule_id = scheduleId;
        ticket.seat_id = seatPos->data.id;
        ticket.price = 50; // 假设默认票价为50
        ticket.status = TICKET_AVL;
        ticket.date = selectedSchedule->data.startdate;
        ticket.time = selectedSchedule->data.starttime;
        
        if (Ticket_Srv_Add(&ticket)) {
            generatedCount++;
        }
    }
    
    printf("Successfully regenerated %d tickets for schedule ID %d!\n", generatedCount, scheduleId);
    printf("Press any key to continue...");
    _getch();
    
    List_Destroy(list, schedule_node_t);
    List_Destroy(existingTickets, ticket_node_t);
    List_Destroy(seatList, seat_node_t);
}

void Ticket_UI_ManageTickets(void) {
    while (1) {
        printf("\n==================== Ticket Management ====================\n");
        printf("1. List all tickets\n");
        printf("2. Add ticket\n");
        printf("3. Delete ticket\n");
        printf("4. Modify ticket\n");
        printf("5. Update ticket status\n");
        printf("0. Return\n");
        printf("======================================================\n");

        int choice = readInt("Input choice: ");

        switch (choice) {
        case 1:
            Ticket_UI_ListTickets();
            break;
        case 2:
            Ticket_UI_AddTicket();
            break;
        case 3:
            Ticket_UI_DeleteTicket();
            break;
        case 4:
            Ticket_UI_ModifyTicket();
            break;
        case 5:
            Ticket_UI_UpdateTicketStatus();
            break;
        case 0:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void Ticket_UI_ListTickets(void) {
    printf("\n--------------------- List Tickets ---------------------.\n");
    
    ticket_list_t list;
    List_Init(list, ticket_node_t);
    int count = Ticket_Srv_FetchAll(list);
    
    if (count <= 0) {
        printf("No tickets available!\n");
        List_Destroy(list, ticket_node_t);
        return;
    }
    
    printf("%-5s %-10s %-10s %-10s %-15s %-15s\n", "ID", "Schedule", "Seat", "Price", "Status", "Date/Time");
    printf("-----------------------------------------------------------\n");
    
    ticket_node_t* pos;
    List_ForEach(list, pos) {
        char statusStr[20];
        switch (pos->data.status) {
        case TICKET_AVL:
            strcpy(statusStr, "Available");
            break;
        case TICKET_SOLD:
            strcpy(statusStr, "Sold");
            break;
        case TICKET_RESV:
            strcpy(statusStr, "Reserved");
            break;
        default:
            strcpy(statusStr, "Unknown");
        }
        
        printf("%-5d %-10d %-10d %-10.2f %-15s %04d-%02d-%02d %02d:%02d\n",
            pos->data.id, pos->data.schedule_id, pos->data.seat_id, pos->data.price,
            statusStr, pos->data.date.year, pos->data.date.month, pos->data.date.day,
            pos->data.time.hour, pos->data.time.minute);
    }
    
    printf("Total tickets: %d\n", count);
    printf("Press any key to continue...");
    _getch();
    List_Destroy(list, ticket_node_t);
}

void Ticket_UI_AddTicket(void) {
    printf("\n--------------------- Add Ticket ---------------------.\n");
    
    ticket_t ticket;
    ticket.id = 0; // 自动生成
    
    ticket.schedule_id = readInt("Input schedule ID: ");
    ticket.seat_id = readInt("Input seat ID: ");
    ticket.price = readFloat("Input ticket price: ");
    
    printf("Select ticket status:\n");
    printf("1. Available\n");
    printf("2. Reserved\n");
    int statusChoice = readInt("Input choice: ");
    switch (statusChoice) {
    case 1:
        ticket.status = TICKET_AVL;
        break;
    case 2:
        ticket.status = TICKET_RESV;
        break;
    default:
        ticket.status = TICKET_AVL;
    }
    
    // 设置日期和时间为当前时间
    ticket.date = DateNow();
    ticket.time = TimeNow();
    
    if (Ticket_Srv_Add(&ticket)) {
        printf("Ticket added successfully! Ticket ID: %d\n", ticket.id);
    } else {
        printf("Failed to add ticket!\n");
    }
    printf("Press any key to continue...");
    _getch();
}

void Ticket_UI_DeleteTicket(void) {
    printf("\n--------------------- Delete Ticket ---------------------.\n");
    
    int ticketId = readInt("Input ticket ID to delete: ");
    
    if (Ticket_Srv_DeleteByID(ticketId)) {
        printf("Ticket deleted successfully!\n");
    } else {
        printf("Failed to delete ticket!\n");
    }
    printf("Press any key to continue...");
    _getch();
}

void Ticket_UI_ModifyTicket(void) {
    printf("\n--------------------- Modify Ticket ---------------------.\n");
    
    int ticketId = readInt("Input ticket ID to modify: ");
    
    ticket_t ticket;
    if (!Ticket_Srv_FetchByID(ticketId, &ticket)) {
        printf("Ticket not found!\n");
        return;
    }
    
    printf("Current ticket information:\n");
    printf("ID: %d\n", ticket.id);
    printf("Schedule ID: %d\n", ticket.schedule_id);
    printf("Seat ID: %d\n", ticket.seat_id);
    printf("Price: %.2f\n", ticket.price);
    
    // 修改信息
    ticket.schedule_id = readInt("Input new schedule ID (press Enter to keep current): ");
    ticket.seat_id = readInt("Input new seat ID (press Enter to keep current): ");
    ticket.price = readFloat("Input new price (press Enter to keep current): ");
    
    if (Ticket_Srv_Modify(&ticket)) {
        printf("Ticket modified successfully!\n");
    } else {
        printf("Failed to modify ticket!\n");
    }
    printf("Press any key to continue...");
    _getch();
}

void Ticket_UI_UpdateTicketStatus(void) {
    printf("\n--------------------- Update Ticket Status ---------------------.\n");
    
    int ticketId = readInt("Input ticket ID to update status: ");
    
    ticket_t ticket;
    if (!Ticket_Srv_FetchByID(ticketId, &ticket)) {
        printf("Ticket not found!\n");
        return;
    }
    
    printf("Current ticket status: ");
    switch (ticket.status) {
    case TICKET_AVL:
        printf("Available\n");
        break;
    case TICKET_SOLD:
        printf("Sold\n");
        break;
    case TICKET_RESV:
        printf("Reserved\n");
        break;
    default:
        printf("Unknown\n");
    }
    
    printf("Select new status:\n");
    printf("1. Available\n");
    printf("2. Sold\n");
    printf("3. Reserved\n");
    int statusChoice = readInt("Input choice: ");
    
    switch (statusChoice) {
    case 1:
        ticket.status = TICKET_AVL;
        break;
    case 2:
        ticket.status = TICKET_SOLD;
        break;
    case 3:
        ticket.status = TICKET_RESV;
        break;
    default:
        printf("Invalid choice!\n");
        return;
    }
    
    // 更新日期和时间为当前时间
    ticket.date = DateNow();
    ticket.time = TimeNow();
    
    if (Ticket_Srv_Update(&ticket)) {
        printf("Ticket status updated successfully!\n");
    } else {
        printf("Failed to update ticket status!\n");
    }
    printf("Press any key to continue...");
    _getch();
}

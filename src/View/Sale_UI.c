#define _CRT_SECURE_NO_WARNINGS
#include "../Common/List.h"
#include "Sale_UI.h"
#include "../Service/Sale.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Account.h"
#include "../Common/common.h"
#include "../SafeStdin/safe_stdin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern account_t gl_CurUser;

static int Sale_UI_Login(void) {
    char username[20] = { 0 };
    char password[20] = { 0 };
    printf("Login required.\n");
    readString(username, sizeof(username), "Username: ");
    readString(password, sizeof(password), "Password: ");
    if (!Account_Srv_Verify(username, password)) {
        printf("Login failed!\n");
        return 0;
    }
    return 1;
}

int Sale_UI_SellTicket(void) {
    printf("\n================ Sell Ticket ================\n");

    int ticketID = 0;
    ticket_t ticket;
    seat_t seat;

    ticketID = readInt("Input ticket ID to sell: ");

    if (!Ticket_Srv_FetchByID(ticketID, &ticket)) {
        printf("Ticket not found!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    if (ticket.status != TICKET_AVL) {
        printf("Ticket is not available for sale (status=%d)!\n", ticket.status);
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    if (!Seat_Srv_FetchByID(ticket.seat_id, &seat)) {
        printf("Seat not found for this ticket!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    if (seat.status != SEAT_GOOD) {
        printf("Seat is not available (status=%d)!\n", seat.status);
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    ticket.status = TICKET_SOLD;
    ticket.date = DateNow();
    ticket.time = TimeNow();
    if (!Ticket_Srv_Update(&ticket)) {
        printf("Failed to update ticket status!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    seat.status = SEAT_NONE;
    if (!Seat_Srv_Modify(&seat)) {
        ticket.status = TICKET_AVL;
        Ticket_Srv_Update(&ticket);
        printf("Failed to update seat status!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    sale_t sale;
    memset(&sale, 0, sizeof(sale));
    sale.user_id = gl_CurUser.id;
    sale.ticket_id = ticket.id;
    sale.price = (int)ticket.price;
    {
        user_date_t d = DateNow();
        user_time_t t = TimeNow();
        sprintf(sale.sale_time, "%04d-%02d-%02d %02d:%02d:%02d",
            d.year, d.month, d.day, t.hour, t.minute, t.second);
    }

    if (Sale_Srv_Add(&sale) <= 0) {
        ticket.status = TICKET_AVL;
        Ticket_Srv_Update(&ticket);
        seat.status = SEAT_GOOD;
        Seat_Srv_Modify(&seat);
        printf("Failed to insert sale record!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    printf("Sell success! TicketID=%d SeatID=%d\n", ticket.id, ticket.seat_id);
    printf("Press any key to continue...");
    _getch();
    return 1;
}

int Sale_UI_ReturnTicket(void) {
    printf("\n================ Return Ticket ================\n");

    int ticketID = 0;
    ticket_t ticket;
    seat_t seat;

    ticketID = readInt("Input ticket ID to return: ");

    if (!Ticket_Srv_FetchByID(ticketID, &ticket)) {
        printf("Ticket not found!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    if (ticket.status != TICKET_SOLD) {
        printf("Ticket is not sold yet (status=%d)!\n", ticket.status);
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    ticket.status = TICKET_AVL;
    ticket.date = DateNow();
    ticket.time = TimeNow();
    if (!Ticket_Srv_Update(&ticket)) {
        printf("Failed to update ticket status!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    if (!Seat_Srv_FetchByID(ticket.seat_id, &seat)) {
        printf("Seat not found for this ticket!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    seat.status = SEAT_GOOD;
    if (!Seat_Srv_Modify(&seat)) {
        printf("Failed to update seat status!\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    sale_list_t list;
    List_Init(list, sale_node_t);
    Sale_Srv_FetchAll(list);
    sale_node_t* pos;
    int deleted = 0;
    List_ForEach(list, pos) {
        if (pos->data.ticket_id == ticketID) {
            if (Sale_Srv_DeleteByID(pos->data.id)) {
                deleted = 1;
            }
            break;
        }
    }
    List_Destroy(list, sale_node_t);
    if (!deleted) {
        printf("Warning: no sale record found for TicketID=%d\n", ticketID);
    }

    printf("Return success! TicketID=%d SeatID=%d\n", ticket.id, ticket.seat_id);
    printf("Press any key to continue...");
    _getch();
    return 1;
}

void Sale_UI_QuerySale(void) {
    printf("\n================ Query Sale ================\n");
    sale_list_t list;
    List_Init(list, sale_node_t);
    int count = Sale_Srv_FetchAll(list);
    printf("Total: %d sales\n", count);
    sale_node_t* pos;
    List_ForEach(list, pos) {
        printf("ID:%d TicketID:%d Price:%d\n", pos->data.id, pos->data.ticket_id, pos->data.price);
    }
    List_Destroy(list, sale_node_t);
    printf("Press any key to continue...");
    _getch();
}

void Sale_UI_MgtEntry(void) {
    char choice[10];
    do {
        system("cls");
        printf("\n================ Sale Management ===============-\n");
        printf("[S]ell Ticket\n");
        printf("[R]eturn Ticket\n");
        printf("[Q]uery Sale\n");
        printf("[E]xit\n");
        readString(choice, sizeof(choice), "Please input your choice: ");
        switch (choice[0]) {
        case 'S': case 's': Sale_UI_SellTicket(); break;
        case 'R': case 'r': Sale_UI_ReturnTicket(); break;
        case 'Q': case 'q': Sale_UI_QuerySale(); break;
        }
    } while (choice[0] != 'E' && choice[0] != 'e');
}

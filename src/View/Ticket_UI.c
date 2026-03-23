#define _CRT_SECURE_NO_WARNINGS
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>

void Ticket_UI_Main(void) {
    while (1) {
        printf("\n==================== Ticket Management ====================\n");
        printf("1. Generate tickets for schedule\n");
        printf("2. Regenerate tickets for schedule\n");
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
        case 0:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void Ticket_UI_GenTicket(void) {
    printf("\n--------------------- Generate Tickets ---------------------\n");
    printf("Function not implemented yet.\n");
}

void Ticket_UI_ReGenTicket(void) {
    printf("\n------------------- Regenerate Tickets -------------------\n");
    printf("Function not implemented yet.\n");
}

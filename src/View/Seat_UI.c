#define _CRT_SECURE_NO_WARNINGS
#include "Seat_UI.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

static HANDLE hConsole = NULL;

static void Seat_UI_InitConsole(void) {
    if (hConsole == NULL) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
}

static void Seat_UI_SetColor(int color) {
    Seat_UI_InitConsole();
    SetConsoleTextAttribute(hConsole, color);
}

static void Seat_UI_ResetColor(void) {
    Seat_UI_SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

static void Seat_UI_PrintColorBlock(int color) {
    Seat_UI_SetColor(color);
    printf("  ");
    Seat_UI_ResetColor();
}

char Seat_UI_Status2Char(seat_status_t status) {
    switch (status) {
    case SEAT_GOOD:    return '#';
    case SEAT_BROKEN:  return 'x';
    case SEAT_NONE:    return ' ';
    default:           return '?';
    }
}

seat_status_t Seat_UI_Char2Status(char statusChar) {
    switch (statusChar) {
    case '#': return SEAT_GOOD;
    case 'x': return SEAT_BROKEN;
    case ' ': return SEAT_NONE;
    default:  return SEAT_NONE;
    }
}

static void Seat_UI_PrintMatrix(seat_list_t list, int rows, int cols) {
    int r, c;
    seat_node_t* pos;

    printf("\n==================== Seat Matrix ====================\n");
    printf("    ");
    for (c = 1; c <= cols; c++) printf("%-4d", c);
    printf("\n");

    for (r = 1; r <= rows; r++) {
        printf("%-3d", r);
        for (c = 1; c <= cols; c++) {
            seat_status_t status = SEAT_NONE;
            List_ForEach(list, pos) {
                if (pos->data.row == r && pos->data.column == c) {
                    status = pos->data.status;
                    break;
                }
            }
            switch (status) {
            case SEAT_GOOD:
                Seat_UI_PrintColorBlock(BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                break;
            case SEAT_BROKEN:
                Seat_UI_PrintColorBlock(BACKGROUND_RED | BACKGROUND_INTENSITY);
                break;
            case SEAT_NONE:
            default:
                Seat_UI_PrintColorBlock(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                break;
            }
            printf("  ");
        }
        printf("\n\n");
    }
    printf("======================================================\n");
    printf("Status: ");
    Seat_UI_SetColor(BACKGROUND_GREEN | BACKGROUND_INTENSITY);
    printf("  ");
    Seat_UI_ResetColor();
    printf(" Good  ");
    Seat_UI_SetColor(BACKGROUND_RED | BACKGROUND_INTENSITY);
    printf("  ");
    Seat_UI_ResetColor();
    printf(" Broken  ");
    Seat_UI_SetColor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    printf("  ");
    Seat_UI_ResetColor();
    printf(" None\n");
}

void Seat_UI_MgtEntry(int roomID) {
    char choice[10] = { 0 };
    int row, col;
    studio_t studio;

    if (!Studio_Srv_FetchByID(roomID, &studio)) {
        printf("Studio not found!\nPress Enter to return...");
        char temp[10] = { 0 };
        readString(temp, sizeof(temp), "");
        return;
    }

    seat_list_t seatList;
    List_Init(seatList, seat_node_t);
    Seat_Srv_FetchByRoomID(seatList, roomID);

    do {
        Seat_UI_PrintMatrix(seatList, studio.rowsCount, studio.colsCount);
        printf("\n[A]dd Seat | [M]odify Seat | [D]elete Seat | [R]eturn\n");
        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 'a': case 'A':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Add(seatList, roomID, row, col)) {
                printf("Add success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Add failed!\n");
            }
            break;

        case 'm': case 'M':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Modify(seatList, row, col)) {
                printf("Modify success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Modify failed!\n");
            }
            break;

        case 'd': case 'D':
            row = readInt("Input Row:");
            col = readInt("Input Column:");
            if (Seat_UI_Delete(seatList, row, col)) {
                printf("Delete success!\n");
                List_Free(seatList, seat_node_t);
                Seat_Srv_FetchByRoomID(seatList, roomID);
            }
            else {
                printf("Delete failed!\n");
            }
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');

    List_Destroy(seatList, seat_node_t);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {
    seat_t rec;
    memset(&rec, 0, sizeof(seat_t));
    char statusChar[10] = { 0 };

    rec.roomID = roomID;
    rec.row = row;
    rec.column = column;

    printf("Input Seat Status (#=Good, x=Broken,  =None):");
    readString(statusChar, sizeof(statusChar), "");
    rec.status = Seat_UI_Char2Status(statusChar[0]);

    return Seat_Srv_Add(&rec);
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
    seat_node_t* pos;
    char statusChar[10] = { 0 };

    List_ForEach(list, pos) {
        if (pos->data.row == row && pos->data.column == column) {
            printf("Current Status: %c\n", Seat_UI_Status2Char(pos->data.status));
            printf("Input New Status (#=Good, x=Broken,  =None):");
            readString(statusChar, sizeof(statusChar), "");
            pos->data.status = Seat_UI_Char2Status(statusChar[0]);
            return Seat_Srv_Modify(&pos->data);
        }
    }

    printf("Seat not found!\n");
    return 0;
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {
    seat_node_t* pos;

    List_ForEach(list, pos) {
        if (pos->data.row == row && pos->data.column == column) {
            return Seat_Srv_DeleteByID(pos->data.id);
        }
    }

    printf("Seat not found!\n");
    return 0;
}

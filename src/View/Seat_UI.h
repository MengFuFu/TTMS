#ifndef SEAT_UI_H_
#define SEAT_UI_H_
#include "../Common/List.h"
#include "../Service/Seat.h"

void Seat_UI_MgtEntry(int roomID);
char Seat_UI_Status2Char(seat_status_t status);
seat_status_t Seat_UI_Char2Status(char statusChar);
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column);
int Seat_UI_Modify(seat_list_t list, int row, int column);
int Seat_UI_Delete(seat_list_t list, int row, int column);

#endif

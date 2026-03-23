#ifndef SCHEDULE_UI_H_
#define SCHEDULE_UI_H_

#include "../Service/Schedule.h"

static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_MgtEntry(int play_id);
int Schedule_UI_Add(int play_id);
int Schedule_UI_Modify(int id);
int Schedule_UI_Delete(int id);
void Schedule_UI_ListAll(void);
int Schedule_UI_Query(char* play_name);

#endif

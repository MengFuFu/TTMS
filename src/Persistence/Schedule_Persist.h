/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Schedule_Persist.h
* Description: Schedule persistence layer header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include "../Service/Schedule.h"
#include "../Service/Play.h"

int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);
int Schedule_Perst_Insert(schedule_t* data);
int Schedule_Perst_Update(const schedule_t* data);
int Schedule_Perst_DeleteByID(int id);
int Schedule_Perst_SelectAll(schedule_list_t list);
int Schedule_Perst_SelectByName(play_list_t list, char condt[]);

#endif

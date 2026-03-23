/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Schedule.c
* Description: Schedule business logic implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include <stdlib.h>
#include <assert.h>

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id){
    int rtn = Schedule_Perst_SelectByPlay(list, play_id);
    return rtn;
}

int Schedule_Srv_Add(schedule_t* data)
{
    int rtn = Schedule_Perst_Insert(data);
    return rtn;
}

int Schedule_Srv_Modify(const schedule_t* data)
{
    int rtn = Schedule_Perst_Update(data);
    return rtn;
}

int Schedule_Srv_DeleteByID(int id)
{
    int rtn = Schedule_Perst_DeleteByID(id);
    return rtn;
}

int Schedule_Srv_FetchAll(schedule_list_t list)
{
    return Schedule_Perst_SelectAll(list);
}

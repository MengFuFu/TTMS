/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Schedule.h
* Description: Schedule business logic header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/common.h"
#include "../Service/Play.h"

typedef struct {
    int id;
    char name[30];
    char studio[30];
    user_date_t startdate;
    user_time_t starttime;
    user_date_t enddate;
    user_time_t endtime;
} schedule_t;

typedef struct studio_node {
    schedule_t data;
    struct studio_node* next;
    struct studio_node* prev;
} schedule_node_t, * schedule_list_t;

typedef enum {
    SCHEDULE_NOT_STARTED = 0,
    SCHEDULE_ONGOING = 1,
    SCHEDULE_ENDED = 2,
    SCHEDULE_CANCELLED = 3
} schedule_status_t;

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);
int Schedule_Srv_Add(schedule_t* data);
int Schedule_Srv_Modify(const schedule_t* data);
int Schedule_Srv_DeleteByID(int id);
int Schedule_Srv_FetchAll(schedule_list_t list);
int Schedule_Srv_FetchByName(play_list_t list, char condt[]);

#endif

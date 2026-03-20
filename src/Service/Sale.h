/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale.h
* Description : 售票业务逻辑层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SALE_H_
#define SALE_H_

#include "../Common/List.h"
#include "../Common/common.h"
//#include "Ticket.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"
//#include "../"

#define SALE_DATA_FILE "Sale.dat"
#define SCHEDULE_DATA_FILE "Schedule.dat"

//typedef enum {
//    SCHEDULE_NOT_STARTED = 0,
//    SCHEDULE_ONGOING = 1,
//    SCHEDULE_ENDED = 2,
//    SCHEDULE_CANCELLED = 3
//} schedule_status_t;
//
//typedef struct {
//    int id;
//    int play_id;
//    int studio_id;
//    char start_time[20];
//    char end_time[20];
//    int price;
//    int seat_count;
//    schedule_status_t status;
//} schedule_t;

typedef struct {
    int id;
    int user_id;
    int ticket_id;
    char sale_time[20];
    float price;
} sale_t;

//typedef struct schedule_node {
//    schedule_t data;
//    struct schedule_node *next;
//    struct schedule_node *prev;
//} schedule_node_t, *schedule_list_t;

typedef struct sale_node {
    sale_t data;
    struct sale_node *next;
    struct sale_node *prev;
} sale_node_t, *sale_list_t;

int Sale_Srv_Add(const sale_t *data);
int Sale_Srv_DeleteByID(int id);
int Sale_Srv_FetchByID(int id, sale_t *buf);
int Sale_Srv_FetchAll(sale_list_t list);

//int Ticket_Srv_Modify(const ticket_t *data);
//int Ticket_Srv_FetchBySchID(int id, ticket_list_t list);
//int Ticket_Srv_FetchAll(ticket_list_t list);
//ticket_node_t *Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id);
//int Ticket_Srv_FetchByID(int id, ticket_t *buf);
//
//int Schedule_Srv_Add(schedule_t *data);
//int Schedule_Srv_Modify(const schedule_t *data);
//int Schedule_Srv_DeleteByID(int id);
//int Schedule_Srv_FetchByID(int id, schedule_t *buf);
//int Schedule_Srv_FetchByPlayID(schedule_list_t list, int playID);
//int Schedule_Srv_FetchAll(schedule_list_t list);
//
//void Play_Srv_FilterByName(schedule_list_t list, const char *name);

#endif

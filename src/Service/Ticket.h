#pragma once
#include <stdlib.h>
#include "../Common/List.h"
#include "../Common/common.h"


enum TICK_ERR {
	TICK_SUCCESS = 0,
	TICK_INIT_ERR,
	TICK_CODE_ERR
};

//状态定义
typedef enum {
	TICKET_AVL = 0,		//待售
	TICJET_SOLD  = 1,	//已售
	TICKET_RESV = 9		//预留
}ticket_status_t;

//票类型定义f
typedef struct Ticket {
	int id;					//票id
	int schedule_id;		//演出计划id
	int seat_id;			//座位id
	int price;			    //票价
	ticket_status_t status;	//票状态
}ticket_t;


// 链表节点定义
typedef struct ticket_node {
	ticket_t data;
	struct ticket_node* prev, * next;
} ticket_node_t, * ticket_list_t;

// 链表头
ticket_list_t ticket_list = NULL;

// 初始化链表
int Ticket_List_Init(ticket_list_t list);

//初始化,并将一个链表节点直接加入票务管理链表
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price);

//加入一批新产生的票
int Ticket_Srv_Batch_Add(int schedule_id, int price);

//删
int Ticet_Srv_Batch_Delete();

//查
int Ticket_Srv_Status(int schedule_id, int seat_id);

//改
int Ticket_Srv_Status(int schedule_id, int seat_id);

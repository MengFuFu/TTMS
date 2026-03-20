#pragma once

#include <stdlib.h>
#include "../Common/List.h"
#include "../Common/common.h"

// ������
enum TICK_ERR {
	TICK_SUCCESS = 0,
	TICK_INIT_ERR,
	TICK_CODE_ERR,
	TICK_LIST_EMPTY,
	TICK_NO_EXIST
};

// 票状态
typedef enum {
	TICKET_AVL = -1,   // 可售
	TICKET_SOLD = -2,  // 已售
	TICKET_RESV = -3,  // 已预订
	TICKET_RETURNED = -4 // 已退票
} ticket_status_t;

// Ʊ�ṹ��
typedef struct Ticket {
	int id;
	int schedule_id;
	int seat_id;
	int price;
	ticket_status_t status;
} ticket_t;



// �����ڵ�
typedef struct ticket_node {
	ticket_t data;
	struct ticket_node* prev, * next;
} ticket_node_t, * ticket_list_t;


extern ticket_list_t ticket_list;
// ===================== �������� =====================
int Ticket_List_Init(ticket_list_t list);
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price);
int Ticket_Srv_Batch_Add(ticket_list_t head, int schedule_id, int price);
int Ticket_Srv_Batch_Delete(ticket_list_t head);
int Ticket_Srv_Check_Status(ticket_list_t head, int schedule_id, int seat_id);
int Ticket_Srv_Mov_Status(ticket_list_t head, int schedule_id, int seat_id, int status);
int Ticket_Srv_DeleteByScheduleID(ticket_list_t head, int schedule_id);
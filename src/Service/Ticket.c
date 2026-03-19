#include "Ticket.h"
#include <stdlib.h>
//初始化

int Ticket_List_Init(ticket_list_t list) {
	//TODO：初始化当前链表
}

//TODO：返回值清晰化
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price) {
	ticket_node_t* newnode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
	if (newnode == NULL) {
		return TICK_INIT_ERR;
	}
	newnode->data.price = price;
	newnode->data.schedule_id = schedule_id;
	newnode->data.seat_id = seat_id;
	int code = EntKey_Perst_GetNewKeys("ticket", 1);
	if (code == 0) {
		return TICK_CODE_ERR;
	}
	newnode->data.id = code;
	List_AddTail(head, newnode);
	return TICK_SUCCESS;
}

//增
int Ticket_Srv_Batch_Add(int schedule_id, int price) {
	int studio_id = func(schedule_id);//TODO：获取剧院id
	int studio_num
		for()
}

//删
int Ticet_Srv_Batch_Delete();

//查
int Ticket_Srv_Status(int schedule_id, int seat_id);

//改
int Ticket_Srv_Status(int schedule_id, int seat_id);
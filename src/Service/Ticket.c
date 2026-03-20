#include "Ticket.h"
#include <stdlib.h>
//初始化

ticket_list_t ticket_list = NULL;

//初始票链表
int Ticket_List_Init(ticket_list_t list) {
	return Ticket_Perst_SelectAll(list);
}

//初始化一个节点，并且直接存进文件
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price) {
	ticket_node_t* newnode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
	if (newnode == NULL) {
		return TICK_INIT_ERR;
	}
	newnode->data.price = price;
	newnode->data.schedule_id = schedule_id;
	newnode->data.seat_id = seat_id;
	int insert_ret = Ticket_Perst_Insert(&(newnode->data));
	if (insert_ret == 0) {
		return TICK_CODE_ERR;
	}
	newnode->data.id = insert_ret;
	List_AddTail(head, newnode);
	return TICK_SUCCESS;
}

int Ticket_Srv_Batch_Add(ticket_list_t head, int schedule_id, int price) {
	//int studio_id = func(schedule_id);//TODO：获取剧院id
	//int studio_num = func(studio_id);//TODO：获取剧院大小
	int seat_begin_id, seat_end_id;
	//int ret = func(&seat_begin_id, &seat_end_id); 
	////TODO:错误处理
	//for (int i = seat_begin_id; i <=   seat_end_id; i++) {
	//	if (func(studio_id/*, 座位id*/)) {
	//		//剧院对应的座位合法，生成票
	//		int init_ret = Ticket_Srv_Init(head, schedule_id, i, price);
	//		if (init_ret != 0) {
	//			return init_ret;
	//		}
	//	}
	//}
	return TICK_SUCCESS;
}


  


// 通过时间删除过期票
int Ticket_Srv_Batch_Delete(ticket_list_t head) {
	//assert(head != NULL); 

	//if (List_IsEmpty(head)) {
	//	return TICK_LIST_EMPTY;
	//}

	//ticket_node_t* cur, * tmp;
	//int count = 0;

	//cur = head->next;
	//while (cur != head) {
	//	tmp = cur->next;

	//	// func = 判断场次是否已过期
	//	if (func(cur->data.schedule_id)) {
	//		// 先删文件
	//		Ticket_Perst_DeleteByID(cur->data.id);
	//		// 再删内存
	//		List_FreeNode(cur);
	//		count++;
	//	}

	//	cur = tmp;
	//}

	//return count;
}

//查座位对应票的
int Ticket_Srv_Check_Status(ticket_list_t head, int schedule_id, int seat_id) {
	if (List_IsEmpty(head)) {
		return TICK_LIST_EMPTY;
	}
	ticket_node_t* cur;
	List_ForEach(head, cur) {
		if (cur->data.schedule_id == schedule_id && cur->data.seat_id == seat_id) {
			break;
		}
	}
	if (cur == NULL) {
		return TICK_NO_EXIST;
	}
	return cur->data.status;
}

//改
int Ticket_Srv_Mov_Status(ticket_list_t head, int schedule_id, int seat_id, int status) {
	if (List_IsEmpty(head)) {
		return TICK_LIST_EMPTY;
	}
	ticket_node_t* cur;
	List_ForEach(head, cur) {
		if (cur->data.schedule_id == schedule_id && cur->data.seat_id == seat_id) {
			break;
		}
	}
	if (cur == NULL) {
		return TICK_NO_EXIST;
	}
	cur->data.status = status;
	Ticket_Perst_Update(cur->data);
	return TICK_SUCCESS;
}

//通过场次删除
int Ticket_Srv_DeleteByScheduleID(ticket_list_t head, int schedule_id) {
	assert(head != NULL);

	if (List_IsEmpty(head)) {
		return TICK_LIST_EMPTY;
	}

	ticket_node_t* cur, * tmp;
	int count = 0;

	cur = head->next;
	while (cur != head) {
		tmp = cur->next;

		if (cur->data.schedule_id == schedule_id) {
			// 先删文件
			Ticket_Perst_DeleteByID(cur->data.id);
			// 再删内存
			List_FreeNode(cur);
			count++;
		}

		cur = tmp;
	}

	return count;
}
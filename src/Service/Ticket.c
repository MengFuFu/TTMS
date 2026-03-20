#include "Ticket.h"
#include "../Persistence/Ticket_Persist.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// 全局票务链表（适配�?head 参数的调用）
ticket_list_t ticket_list = NULL;

// 初始化票务链�?
int Ticket_List_Init(ticket_list_t list) {
	return Ticket_Perst_SelectAll(list);
}

// 初始化一个新节点，并且直接写入文�?
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price) {
	ticket_node_t* newnode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
	if (newnode == NULL) {
		return TICK_INIT_ERR;
	}
	newnode->data.price = price;
	newnode->data.schedule_id = schedule_id;
	newnode->data.seat_id = seat_id;
	newnode->data.status = TICKET_AVL; // 默认未售/可用

	int insert_ret = Ticket_Perst_Insert(&(newnode->data));
	if (insert_ret == 0) {
		free(newnode);
		return TICK_CODE_ERR;
	}
	newnode->data.id = insert_ret;
	List_AddTail(head, newnode);
	return TICK_SUCCESS;
}

int Ticket_Srv_Batch_Add(ticket_list_t head, int schedule_id, int price) {
	//int studio_id = func(schedule_id);//TODO获取剧院id
	//int studio_num = func(studio_id);//TODO获取剧院大小
	int seat_begin_id, seat_end_id;
	//int ret = func(&seat_begin_id, &seat_end_id); 
	////TODO:循环
	//for (int i = seat_begin_id; i <=   seat_end_id; i++) {
	//	if (func(studio_id/*, 座位id*/)) {
	//		//剧院对应座位上都创建该票

	//		int init_ret = Ticket_Srv_Init(head, schedule_id, i, price);
	//		if (init_ret != 0) {
	//			return init_ret;
	//		}
	//	}
	//}
	return TICK_SUCCESS;
}

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

	//	// func = 判断场次是否已过�?
	//	if (func(cur->data.schedule_id)) {
	//		// 删文�?
	//		Ticket_Perst_DeleteByID(cur->data.id);
	//		// 删内�?
	//		List_FreeNode(cur);
	//		count++;
	//	}

	//	cur = tmp;
	//}

	//return count;
}

//查座位对应票�?
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

//�?
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
	Ticket_Perst_Update(&(cur->data));
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
			// 删文�?
			Ticket_Perst_DeleteByID(cur->data.id);
			// 删内�?
			List_FreeNode(cur);
			count++;
		}

		cur = tmp;
	}

	return count;
}

// ------------------- 【关键】适配�?Sale_UI.c 调用的函数实�?-------------------
// 1. Ticket_Srv_FetchByID：仅�?ticket_id 查找（适配 1 个参数调用）
ticket_node_t* Ticket_Srv_FetchByID(int ticket_id) {
	if (!ticket_list) return NULL;

	ticket_node_t* cur;
	List_ForEach(ticket_list, cur) {
		if (cur->data.id == ticket_id) return cur;
	}
	return NULL;
}

// 2. Ticket_Srv_FetchBySeatID：仅�?seat_id 查找（适配 2 个参数调用，忽略 schedule_id�?
ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t head, int seat_id) {
	if (List_IsEmpty(head)) return NULL;

	ticket_node_t* cur;
	List_ForEach(head, cur) {
		if (cur->data.seat_id == seat_id) return cur;
	}
	return NULL;
}

// 3. Ticket_Srv_Modify：仅�?data->id 修改（适配 1 个参数调用，用全局链表�?
int Ticket_Srv_Modify(const ticket_t* data) {
	if (!data || !ticket_list) return TICK_PARAM_ERR;

	ticket_node_t* cur;
	List_ForEach(ticket_list, cur) {
		if (cur->data.id == data->id) {
			cur->data = *data;
			Ticket_Perst_Update(&(cur->data));
			return TICK_SUCCESS;
		}
	}
	return TICK_NO_EXIST;
}

// 4. Ticket_Srv_FetchByScheduleID：按 schedule_id 查找（给别名用）
ticket_node_t* Ticket_Srv_FetchByScheduleID(ticket_list_t head, int schedule_id) {
	if (List_IsEmpty(head)) return NULL;

	ticket_node_t* cur;
	List_ForEach(head, cur) {
		if (cur->data.schedule_id == schedule_id) return cur;
	}
	return NULL;
}
// -----------------------------------------------------------------------------------
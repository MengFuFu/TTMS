/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02
* File name: Seat.h
* Description : 设置座位用例持久化层
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

//添加对象主键标识名称
static const char SEAT_KEY_NAME[] = "Seat";

/*
函数功能：用于向文件中添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_Perst_Insert(seat_t* data) {
	assert(NULL != data);

	FILE* fp = fopen(SEAT_DATA_FILE, "ab");
	if (!fp) return 0;

	int rtn = fwrite(data, sizeof(seat_t), 1, fp);
	fclose(fp);
	return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_InsertBatch
函数功能：用于向文件中添加一批座位数据。
参数说明：list为seat_list_t类型，表示需要添加的一批座位的链表头指针。
返 回 值：整型，表示成功添加一批座位的个数。
*/
int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t* p;
	assert(NULL != list);

	int count = 0;
	List_ForEach(list, p) {
		if (Seat_Perst_Insert(&p->data)) {
			count++;
		}
	}
	return count;
}

/*
标识符：TTMS_SCU_Seat_Perst_Update
函数功能：用于在文件中更新一个座位数据。
参数说明：data为seat_t类型指针，表示需要更新的座位数据结点。
返 回 值：整型，表示是否成功更新了座位的标志。
*/
int Seat_Perst_Update(const seat_t* seatdata) {
	assert(NULL != seatdata);

	FILE* fp = fopen(SEAT_DATA_FILE, "rb+");
	if (!fp) return 0;

	seat_t buf;
	int found = 0;
	while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
		if (buf.id == seatdata->id) {
			fseek(fp, -(long)sizeof(seat_t), SEEK_CUR);
			fwrite(seatdata, sizeof(seat_t), 1, fp);
			found = 1;
			break;
		}
	}
	fclose(fp);
	return found;
}

/*
识符：TTMS_SCU_Seat_Perst_DelByID
函数功能：用于从文件中删除一个座位的数据。
参数说明：参数ID为整型，表示需要删除的座位ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteByID(int ID) {
	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	FILE* tmp = fopen(SEAT_DATA_TEMP_FILE, "wb");
	if (!fp || !tmp) {
		if (fp) fclose(fp);
		if (tmp) fclose(tmp);
		return 0;
	}

	seat_t buf;
	int found = 0;
	while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
		if (buf.id != ID) {
			fwrite(&buf, sizeof(seat_t), 1, tmp);
		}
		else {
			found = 1;
		}
	}
	fclose(fp);
	fclose(tmp);
	remove(SEAT_DATA_FILE);
	rename(SEAT_DATA_TEMP_FILE, SEAT_DATA_FILE);
	return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_DelAllByID
函数功能：根据编号用于从文件中删除座位数据。
参数说明：参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteAllByRoomID(int roomID) {
	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	FILE* tmp = fopen(SEAT_DATA_TEMP_FILE, "wb");
	if (!fp || !tmp) {
		if (fp) fclose(fp);
		if (tmp) fclose(tmp);
		return 0;
	}

	seat_t buf;
	int found = 0;
	while (fread(&buf, sizeof(seat_t), 1, fp) == 1) {
		if (buf.roomID != roomID) {
			fwrite(&buf, sizeof(seat_t), 1, tmp);
		}
		else {
			found = 1;
		}
	}
	fclose(fp);
	fclose(tmp);
	remove(SEAT_DATA_FILE);
	rename(SEAT_DATA_TEMP_FILE, SEAT_DATA_FILE);
	return found;
}

/*
标识符：TTMS_SCU_Studio_Perst_SelByID
函数功能：用于从文件中载入一个座位的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的座位ID；第二个参数buf为seat_t指针，指向载入座位数据的指针。
返 回 值：整型，表示是否成功载入了座位的标志。
*/
int Seat_Perst_SelectByID(int ID, seat_t* buf) {
	assert(NULL != buf);
	memset(buf, 0, sizeof(seat_t));

	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	if (!fp) return 0;

	seat_t temp;
	int found = 0;
	while (fread(&temp, sizeof(seat_t), 1, fp) == 1) {
		if (temp.id == ID) {
			*buf = temp;
			found = 1;
			break;
		}
	}
	fclose(fp);
	return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelAll
函数功能：用于从文件中载入所有座位数据。
参数说明：list为seat_list_t类型，表示将要载入的座位链表头指针。
返 回 值：整型，成功载入座位的个数。
*/
int Seat_Perst_SelectAll(seat_list_t list) {
	assert(NULL != list);
	List_Free(list, seat_node_t);

	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	if (!fp) return 0;

	seat_t data;
	int count = 0;
	seat_node_t* newNode;
	while (fread(&data, sizeof(seat_t), 1, fp) == 1) {
		newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
		if (newNode) {
			memset(newNode, 0, sizeof(seat_node_t));
			newNode->data = data;
			newNode->next = list->next;
			newNode->prev = list;
			if (list->next) list->next->prev = newNode;
			list->next = newNode;
			count++;
		}
	}
	fclose(fp);
	return count;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelByRoomID
函数功能：用于在文件中根据演出厅ID载入所有座位数据。
参数说明：第一个参数list为seat_list_t类型，表示将要载入的座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示成功载入了演出厅座位的个数。
*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
	assert(NULL != list);
	List_Free(list, seat_node_t);

	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	if (!fp) return 0;

	seat_t data;
	int count = 0;
	seat_node_t* newNode;
	while (fread(&data, sizeof(seat_t), 1, fp) == 1) {
		if (data.roomID == roomID) {
			newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
			if (newNode) {
				memset(newNode, 0, sizeof(seat_node_t));
				newNode->data = data;
				newNode->next = list->next;
				newNode->prev = list;
				if (list->next) list->next->prev = newNode;
				list->next = newNode;
				count++;
			}
		}
	}
	fclose(fp);
	return count;
}
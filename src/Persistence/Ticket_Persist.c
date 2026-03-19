#define _CRT_SECURE_NO_WARNINGS



#include "EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./Ticket_Persist.h"

static const char TICKET_DATA_FILE[] = "Studio.dat"; //票务存储文件名常量 
static const char TICKET_DATA_TEMP_FILE[] = "StudioTmp.dat"; //删除票临时文件名常量 
static const char TICKET_KEY_NAME[] = "Ticket"; //票名常量 

//功能：向文件中存储新票 
int Ticket_Perst_Insert(ticket_t* data) {
	assert(NULL != data);

	////以下是新设计方案方案添加的代码
	////以下是新设计方案方案添加的代码
	long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, 1); //为新演出厅分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	////以上是新设计方案方案添加的代码
	////以上是新设计方案方案添加的代码



	FILE* fp = fopen(TICKET_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(ticket_t), 1, fp);

	fclose(fp);
	return rtn;
}

//功能：在文件中更新票
int Ticket_Perst_Update(const ticket_t* data) {
	assert(NULL != data);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;  
	}

	ticket_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(ticket_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(ticket_t)), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

//功能：在文件中删除票
int Ticket_Perst_DeleteByID(int ID) {
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

		//对原始数据文件重命名
	if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n",TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}


	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}

//从文件中载入一条票的数据
int Ticket_Perst_SelectByID(int ID, ticket_t* buf) {
	assert(NULL != buf);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

//功能：从文件中载入全部票
int Ticket_Perst_SelectAll(ticket_list_t list) {
	ticket_node_t* newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, ticket_node_t);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf(
					"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}
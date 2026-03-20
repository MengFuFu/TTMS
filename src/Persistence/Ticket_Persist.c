#define _CRT_SECURE_NO_WARNINGS

#include "EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./Ticket_Persist.h"

// ====================== ֻ�޸�����ļ������� ======================
static const char TICKET_DATA_FILE[] = "Ticket.dat"; //Ʊ��洢�ļ�������?
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat"; //ɾ��Ʊ��ʱ�ļ������� 
// ====================================================================
static const char TICKET_KEY_NAME[] = "Ticket"; //Ʊ������ 

//���ܣ����ļ��д洢��Ʊ 
int Ticket_Perst_Insert(ticket_t* data) {
	assert(NULL != data);

	////����������Ʒ����������ӵĴ���?
	////����������Ʒ����������ӵĴ���?
	long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, 1); //Ϊ���ݳ���������?
	if (key <= 0)			//��������ʧ�ܣ�ֱ�ӷ���
		return 0;

	data->id = key;		//�����¶�����ص�UI��


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

//���ܣ����ļ��и���Ʊ
int Ticket_Perst_Update(const ticket_t* data) {
	assert(NULL != data);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (fread(&buf, sizeof(ticket_t), 1, fp) == 1)
		// ====================================================================
	{
		if (buf.id == data->id) {
			fseek(fp, -((int)sizeof(ticket_t)), SEEK_CUR);
			fwrite(data, sizeof(ticket_t), 1, fp);
			found = 1;
			break;
		}
	}
	fclose(fp);

	return found;
}

//���ܣ����ļ���ɾ��Ʊ
int Ticket_Perst_DeleteByID(int ID) {
	//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", TICKET_DATA_TEMP_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (fread(&buf, sizeof(ticket_t), 1, fpSour) == 1)
		// ====================================================================
	{
		if (ID == buf.id) {
			found = 1;
			continue;
		}
		fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
	}

	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(TICKET_DATA_TEMP_FILE);
	return found;
}

//���ļ�������һ��Ʊ������
int Ticket_Perst_SelectByID(int ID, ticket_t* buf) {
	assert(NULL != buf);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (fread(&data, sizeof(ticket_t), 1, fp) == 1)
		// ====================================================================
	{
		if (ID == data.id) {
			*buf = data;
			found = 1;
			break;
		};
	}
	fclose(fp);

	return found;
}

//���ܣ����ļ�������ȫ��Ʊ
int Ticket_Perst_SelectAll(ticket_list_t list) {
	ticket_node_t* newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, ticket_node_t);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		return 0;
	}

	while (fread(&data, sizeof(ticket_t), 1, fp) == 1)
		// ====================================================================
	{
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
	fclose(fp);
	return recCount;
}
#include <stdio.h>
#include <string.h>
#include "SalesAnalysis.h"
#include "../Persistence/SalesAnalysis_Perst.h"
//#include "Sale.h"
//#include "Ticket.h"
//#include "Play.h"
//#include "Schedule.h"
#include "../Common/list.h"
int Salesanalysis_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate) {
	return 1;
}
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {
	if (list == NULL) {
		return 0;
	}
	play_list_t playList;
	play_node_t *pos;
	salesanalysis_list_t newNode;
	int soldCount = 0;
	int totalCount = 0;
	int rtn = 0;
	List_Free(list, salesanalysis_node_t);
	List_Init(playList, play_node_t);
	Play_Srv_FetchAll(playList);
	List_ForEach(playList, pos) {
		newNode = (salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t));
		if (!newNode) {
			printf("不好意思，内存溢出...\n");
			return;
		}
		strcpy(newNode->data.area, pos->data.area);////????
		newNode->data.duration = pos->data.duration;
		newNode->date.end_date = pos->data.end_date;
		strcpy(newNode->data.name, pos->data.name);
		newNode->date.play_id = pos->data.id;
		newNode->data.price = pos->data.price;
		newNode->date.start_date = pos->data.start_date;
		newNode->date.sales = Schedule_Srv_StatRevByPlay(pos->data.id,&soldCount,&totalCount);
		newNode->data.totaltickets = soldCount;
		List_AddTail(list, newNode);
		Salesanalysis_Perst_Insert(&newNode->data);
		rtn++;
	}
	return rtn++;
}
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list) {
	salesanalysis_node_t* p, * listLeft;
	assert(list != NULL);
	if (List_IsEmpty(list)) {
		return;
	}
	list->prev->next = NULL;
	listLeft = list->next;
	list->next = list->prev = list;
	while (listLeft != NULL) {
		p = listLeft;
		listLeft = listLeft->next;
		SalesAnalysis_Srv_AddToSoftedList(list, p);
	}
}
void SalesAnalysis_Srv_AddToSoftedList(salesanalysis_list_t list, salesanalysis_node_t* node) {
	salesanalysis_node_t* p;
	assert(list != NULL && node != NULL);
	if (List - IsEmpty(list)) {
		List_AddTail(list, node);
	}
	else {
		p = list->next;
		while (p != list && (p->data.sales > node->data.sales)) {
			p = p->next;
		}
		List_InsertBefore(p, node);
	}
}
int StaSales_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate) {
	int amount = 0;
	sale_list_t saleList = NULL;
	sale_node_t pSale;
	List_Init(saleList, sale_node_t);
	if (Sale_Perst_SelectByUserID(saleList, userID, stDate, endDate) <= 0) {//在sale_persist.h里
		return -1;
	}
	sale_list_t pTemp;
	List_ForEach(saleList, pTemp) {
		amount += pTemp->data.value;
	}
	return amount;
}
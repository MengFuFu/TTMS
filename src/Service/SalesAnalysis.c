#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SalesAnalysis.h"
#include "../Persistence/SalesAnalysis_Perst.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/List.h"

int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount, int* totalCount) {
    *soldCount = 0;
    *totalCount = 0;
    return 0;
}

int Salesanalysis_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate) {
    return 0;
}

int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {
    if (list == NULL) {
        return 0;
    }
    play_list_t playList;
    play_node_t* pos;
    salesanalysis_node_t* newNode;
    int soldCount = 0;
    int totalCount = 0;
    int rtn = 0;
    List_Free(list, salesanalysis_node_t);
    List_Init(playList, play_node_t);
    Play_Srv_FetchAll(playList);
    List_ForEach(playList, pos) {
        newNode = (salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));
        if (!newNode) {
            printf("Memory allocation failed!\n");
            break;
        }
        memset(newNode, 0, sizeof(salesanalysis_node_t));
        strcpy(newNode->data.area, pos->data.area);
        newNode->data.duration = pos->data.duration;
        newNode->data.end_date = pos->data.end_date;
        strcpy(newNode->data.name, pos->data.name);
        newNode->data.play_id = pos->data.id;
        newNode->data.price = pos->data.price;
        newNode->data.start_date = pos->data.start_date;
        newNode->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id, &soldCount, &totalCount);
        newNode->data.totaltickets = soldCount;
        newNode->next = newNode->prev = newNode;
        List_AddTail(list, newNode);
        SalesAnalysis_Perst_Insert(&newNode->data);
        rtn++;
    }
    List_Destroy(playList, play_node_t);
    return rtn;
}

void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list) {
    salesanalysis_node_t* p, * listLeft;
    if (list == NULL) return;
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
    if (list == NULL || node == NULL) return;
    if (List_IsEmpty(list)) {
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
    return amount;
}

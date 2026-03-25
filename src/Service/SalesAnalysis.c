#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SalesAnalysis.h"
#include "../Persistence/SalesAnalysis_Perst.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Persistence/Ticket_Persist.h"
#include "Play.h"
#include "Schedule.h"
#include "Ticket.h"
#include "../Common/List.h"

int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount, int* totalCount, int* avgPrice) {
    *soldCount = 0;
    *totalCount = 0;
    *avgPrice = 0;
    int totalSales = 0;
    int totalTicketPrice = 0;
    int ticketCount = 0;
    
    schedule_list_t schedList;
    List_Init(schedList, schedule_node_t);
    
    // 获取该剧目所有的演出计划
    int schedCount = Schedule_Perst_SelectByPlay(schedList, play_id);
    if (schedCount <= 0) {
        List_Destroy(schedList, schedule_node_t);
        return 0;
    }
    
    schedule_node_t* schedPos;
    List_ForEach(schedList, schedPos) {
        ticket_list_t ticketList;
        List_Init(ticketList, ticket_node_t);
        
        // 获取该演出计划的所有票
        int tCount = Ticket_Perst_SelectBySchID(schedPos->data.id, ticketList);
        if (tCount > 0) {
            ticket_node_t* ticketPos;
            List_ForEach(ticketList, ticketPos) {
                (*totalCount)++;
                totalTicketPrice += (int)ticketPos->data.price;
                ticketCount++;
                if (ticketPos->data.status == TICKET_SOLD) {
                    (*soldCount)++;
                    totalSales += (int)ticketPos->data.price;
                }
            }
        }
        List_Destroy(ticketList, ticket_node_t);
    }
    
    List_Destroy(schedList, schedule_node_t);
    
    // 计算平均票价
    if (ticketCount > 0) {
        *avgPrice = totalTicketPrice / ticketCount;
    }
    
    return totalSales;
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
    int avgPrice = 0;
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
        newNode->data.start_date = pos->data.start_date;
        newNode->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id, &soldCount, &totalCount, &avgPrice);
        newNode->data.totaltickets = soldCount;
        newNode->data.price = avgPrice;  // 使用实际票的平均价格
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
        // 按营业额从高到低排序（降序）
        while (p != list && (p->data.sales >= node->data.sales)) {
            p = p->next;
        }
        List_InsertBefore(p, node);
    }
}

int StaSales_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate) {
    int amount = 0;
    return amount;
}

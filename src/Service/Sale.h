/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Sale.h
* Description: Sale service header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SALE_H
#define SALE_H

#include "../Common/List.h"
#include "../Service/Account.h"

// 销售记录实体
typedef struct {
    int id;             // 销售记录ID
    int user_id;        // 售票员ID
    int ticket_id;      // 关联票ID
    int price;          // 售价
    char sale_time[32]; // 销售时间
} sale_t;

// 销售链表节点（必须包含prev和next，适配List宏）
typedef struct sale_node {
    sale_t data;
    struct sale_node* next; // 下一个节点指针
    struct sale_node* prev; // 上一个节点指针（报错的根源就是缺这个）
} sale_node_t;

// 链表头指针类型
typedef sale_node_t* sale_list_t;

// 函数声明
int Sale_Srv_Add(const sale_t* data);
int Sale_Srv_DeleteByID(int id);
int Sale_Srv_FetchAll(sale_list_t list);

#endif // SALE_H
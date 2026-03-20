#ifndef TICKET_H
#define TICKET_H

#include "../Common/List.h"

// 票务状态枚举（和你 Sale_UI.c 完全一致）
typedef enum {
    TICKET_AVL = 1,      // 未售/可用
    TICKET_SOLD,         // 已售
    TICKET_RETURNED,     // 已退
    TICKET_RESV          // 预留
} ticket_status_t;

// 票务实体
typedef struct {
    int id;             // 票ID
    int schedule_id;    // 关联演出计划ID
    int seat_id;        // 关联座位ID
    int price;          // 票价
    int status;         // 票务状态
} ticket_t;

// 链表节点
typedef struct ticket_node {
    ticket_t data;
    struct ticket_node* next;
    struct ticket_node* prev;
} ticket_node_t;

// 链表头指针
typedef ticket_node_t* ticket_list_t;

// 错误码定义
#define TICK_SUCCESS        0
#define TICK_INIT_ERR       1
#define TICK_CODE_ERR       2
#define TICK_LIST_EMPTY     3
#define TICK_NO_EXIST       4
#define TICK_PARAM_ERR      5  // 新增：参数错误

// 初始化票务链表
int Ticket_List_Init(ticket_list_t list);

// 初始化单个票务节点
int Ticket_Srv_Init(ticket_list_t head, int schedule_id, int seat_id, int price);

// 批量添加票务
int Ticket_Srv_Batch_Add(ticket_list_t head, int schedule_id, int price);

// 批量删除过期票务
int Ticket_Srv_Batch_Delete(ticket_list_t head);

// 检查座位对应票务状态
int Ticket_Srv_Check_Status(ticket_list_t head, int schedule_id, int seat_id);

// 修改票务状态
int Ticket_Srv_Mov_Status(ticket_list_t head, int schedule_id, int seat_id, int status);

// 根据场次ID批量删除票务
int Ticket_Srv_DeleteByScheduleID(ticket_list_t head, int schedule_id);

// ------------------- 【关键】完全匹配你 Sale_UI.c 调用参数的函数声明 -------------------
// 1. Ticket_Srv_FetchByID：调用时只传 1 个 int 参数
ticket_node_t* Ticket_Srv_FetchByID(int ticket_id);

// 2. Ticket_Srv_FetchBySeatID：调用时传 2 个参数 (ticket_list_t, int seat_id)
ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t head, int seat_id);

// 3. Ticket_Srv_Modify：调用时只传 1 个 ticket_t* 参数
int Ticket_Srv_Modify(const ticket_t* data);

// 4. 别名：解决 Ticket_Srv_FetchBySchID 未定义报错（和 Ticket_Srv_DeleteByScheduleID 复用）
#define Ticket_Srv_FetchBySchID Ticket_Srv_FetchByScheduleID
ticket_node_t* Ticket_Srv_FetchByScheduleID(ticket_list_t head, int schedule_id);
// -----------------------------------------------------------------------------------

#endif // TICKET_H
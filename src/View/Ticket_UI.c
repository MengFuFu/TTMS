#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../SafeStdin//safe_stdin.h"
#include <stdio.h>

// 票管理主菜单
void Ticket_UI_Main() {
    while (1) {
        printf("\n==================== 演出票管理 ====================\n");
        printf("1. 生成演出票（为场次创建所有座位票）\n");
        printf("2. 重新生成演出票（删除旧票 → 生成新票）\n");
        printf("0. 返回上一级\n");
        printf("======================================================\n");

        int choice = readInt("请输入操作序号：");

        switch (choice) {
        case 1:
            Ticket_UI_GenTicket();
            break;
        case 2:
            Ticket_UI_ReGenTicket();
            break;
        case 0:
            return;
        default:
            printf("输入错误，请重新选择！\n");
        }
    }
}

// 生成演出票
void Ticket_UI_GenTicket() {
    printf("\n--------------------- 生成演出票 ---------------------\n");

    int schedule_id = readInt("请输入场次ID：");
    int price = readInt("请输入票价：");

    // 调用你写的批量生成函数
    int ret = Ticket_Srv_Batch_Add(ticket_list, schedule_id, price);

    if (ret == TICK_SUCCESS) {
        printf("演出票生成成功！\n");
    }
    else {
        printf("演出票生成失败！错误码：%d\n", ret);
    }
}

// 重新生成演出票
void Ticket_UI_ReGenTicket() {
    printf("\n------------------- 重新生成演出票 -------------------\n");

    int schedule_id = readInt("请输入需要重新生成票的场次ID：");
    int price = readInt("请输入新票价：");

    // 先删除该场次旧票
    Ticket_Srv_DeleteByScheduleID(ticket_list, schedule_id);

    // 再重新生成
    int ret = Ticket_Srv_Batch_Add(ticket_list, schedule_id, price);

    if (ret == TICK_SUCCESS) {
        printf("演出票重新生成成功！\n");
    }
    else {
        printf("演出票重新生成失败！错误码：%d\n", ret);
    }
}
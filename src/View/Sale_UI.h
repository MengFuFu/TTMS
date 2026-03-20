/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale_UI.h
* Description : 售票界面层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SALE_UI_H_
#define SALE_UI_H_

#include "../Service/Sale.h"

void Sale_UI_MgtEntry(void);
void Sale_UI_ShowScheduler(int playID);
void Sale_UI_ShowTicket(int scheduleID);
int Sale_UI_SellTicket(int scheduleID);
int Sale_UI_RetfundTicket(void);

#endif

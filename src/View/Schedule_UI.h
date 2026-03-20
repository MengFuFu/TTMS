/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_04	TTMS_UC_06
* File name: Schedule_UI.h
* Description : 演出计划业务界面层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SCHEDULE_UI_H_
#define SCHEDULE_UI_H_

#include"../Service/Schedule.h"

//标识符：TTMS_SCU_Schedule_UI_MgtEnt
//功能：安排演出界面
void Schedule_UI_MgtEntry(int play_id);

//标识符：TTMS_SCU_Schedule_UI_Add
//功能：添加新演出计划界面
int Schedule_UI_Add(int play_id);

//标识符：TTMS_SCU_Schedule_UI_Mod
//功能：修改演出计划界面
int Schedule_UI_Modify(int id);

//标识符：TTMS_SCU_Schedule_UI_Del
//功能：删除演出计划界面
int Schedule_UI_Delete(int id);


//标识符：TTMS_SCU_Schedule_UI_List
//功能：查询演出界面
void Schedule_UI_ListAll(void);

//标识符：TTMS_SCU_Schedule_UI_Qry
//功能：根据剧目名称获取演出计划
int Schedule_UI_Query(char* play_name);

#endif// !SCHEDULE_UI_H_

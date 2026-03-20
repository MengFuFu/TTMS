/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_04	TTMS_UC_06
* File name: Schedule_Persist.h
* Description : 演出计划业务持久化层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include"../Service/Schedule.h"
#include"../Service/Play.h"

//标识符：TTMS_SCU_Schedule_Perst_SelByPlay
//功能：根据剧目ID载入演出计划
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);

//标识符：TTMS_SCU_Schedule_Perst_Insert
//功能：存储新演出计划
int Schedule_Perst_Insert(schedule_t* data);

//标识符：TTMS_SCU_Schedule_Perst_Mod
//功能：更新演出计划
int Schedule_Perst_Update(const schedule_t* data);

//标识符：TTMS_SCU_Schedule_Perst_RemByID
//功能：根据ID去除演出计划
int Schedule_Perst_DeleteByID(int id);

//标识符：TTMS_SCU_Schedule_Perst_SelAll
//功能：载入全部演出计划
int Schedule_Perst_SelectAll(schedule_list_t list);

//标识符：TTMS_SCU_Schedule_Perst_SelByName
//功能：根据名称载入剧目
int Schedule_Perst_SelectByName(play_list_t list,  char condt[]);

#endif// !SCHEDULE_PRESIST_H_
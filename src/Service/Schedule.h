/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_04	TTMS_UC_06
* File name: Schedule.h
* Description : 演出计划业务逻辑层头文件
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include"../Common/common.h"
#include "../Service/Play.h"


//演出计划数据结构 
typedef struct {
	int id;					//演出计划ID
	char name[30];			//剧目名称
	char studio[30];		//演出厅
	user_date_t startdate;	//演出开始日期
	user_time_t starttime;	//演出开始时间
	user_date_t enddate;	//演出结束日期
	user_time_t endtime;	//演出结束时间
} schedule_t;

//双向链表
typedef struct studio_node {
	schedule_t data;			//实体数据
	struct studio_node* next;	//后向指针
	struct studio_node* prev;	//前向指针
} schedule_node_t, * schedule_list_t;
typedef enum {
	    SCHEDULE_NOT_STARTED = 0,
	    SCHEDULE_ONGOING = 1,
	    SCHEDULE_ENDED = 2,
	    SCHEDULE_CANCELLED = 3
	} schedule_status_t;
	
//typedef struct {
//    int id;
//    int play_id;
//    int studio_id;
//    char start_time[20];
//    char end_time[20];
//    int price;
//    int seat_count;
//    schedule_status_t status;
//} schedule_t;


//标识符：TTMS_SCU_Schedule_Srv_FetchByPlay
//功能：根据剧目ID获取演出计划
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);

//标识符：TTMS_SCU_Schedule_Srv_Add
//功能：添加新演出计划
int Schedule_Srv_Add(schedule_t* data);


//标识符：TTMS_SCU_Schedule_Srv_Mod
//功能：修改演出计划
int Schedule_Srv_Modify(const schedule_t* data);

//标识符：TTMS_SCU_Schedule_Srv_DelByID
//功能：根据ID删除演出计划
int Schedule_Srv_DeleteByID(int id);

//标识符：TTMS_SCU_Schedule_Srv_FetchAll
//功能：获取全部演出计划
int Schedule_Srv_FetchAll(schedule_list_t list);

//标识符：TTMS_SCU_Schedule_Srv_FetchByName
//功能：根据名称获取剧目
int Schedule_Srv_FetchByName(play_list_t list, char condt[]);

#endif // !SCHEDULE_H_


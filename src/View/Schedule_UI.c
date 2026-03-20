//
//Copyright(C), 2007-2008, XUPT Univ.
//用例编号：TTMS_UC_04	TTMS_UC_06
//File name: Schedule_UI.h
//Description : 演出计划业务界面层头文件
//Author:   XUPT
//Version:  v.1
//Date: 	2015年4月22日
//

static const int SCHEDULE_PAGE_SIZE = 5;

#define _CRT_SECURE_NO_WARNINGS
#include "Schedule_UI.h"
#include "../Service/Schedule.h"
#include <stdio.h>
#include "../Common/List.h"
//
//标识符：TTMS_SCU_Schedule_UI_MgtEnt
//函数声明：void Schedule_UI_MgtEntry(int play_id);
//函数功能：显示与ID号为play_id的剧目相关的所有演出计划，并提供增，删，改演出计划的功能。
//参数说明：play_id为整型，表示与演出计划相关的剧目ID号。
//返回值：无
//
void Schedule_UI_MgtEntry(int play_id)
{
	play_t buf;
	if (Play_Srv_FetchByID(play_id, &buf) == 0) {
		printf("No such play!\n");
		return;
	}

	int i, id;
	char choice;

	/* 使用 list 作为当前剧目的演出计划链表 */
	schedule_list_t list;
	schedule_node_t* pos;
	Pagination_t paging;

	List_Init(list, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	// 载入与指定 play_id 相关的数据到 list
	
	paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
	Paging_Locate_FirstPage(list, paging);

	do {
		printf("\n==================================================================\n");
		printf("********************** Projection Room List **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "Name", "Studio",
			"Starttime", "Endtime");
		printf("------------------------------------------------------------------\n");
		// 显示数据（按分页）
		Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i) {
			printf("%5d  %18s  %18s\n", pos->data.id, pos->data.name, pos->data.studio);
			printf("%dyear %dmonth %dday %02d:%02d\n",
				pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
				pos->data.starttime.hour, pos->data.starttime.minute);
			printf("%dyear %dmonth %dday %02d:%02d\n",
				pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
				pos->data.endtime.hour, pos->data.endtime.minute);
		}

		printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [R]eturn\n");
		printf("==================================================================\n");
		printf("Your Choice: ");
		scanf(" %c", &choice); // 前导空格跳过残留换行或空格

		switch (choice) {
		case 'a':
		case 'A':
			// 增加演出计划：传入当前 play_id
			if (Schedule_UI_Add(play_id)) { // 新添加成功，跳到最后一页显示
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				Paging_Locate_LastPage(list, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID: ");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	// 删除成功，重新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID: ");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	// 修改成功，重新获取与编号play_id剧目相关的演出计划信息
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
		case 'p':
		case 'P'://上一页
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N'://下一页
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
			}
			break;
		case 'r':
		case 'R':
			/* 退出管理界面 */
			break;
		default:
			printf("Invalid choice.\n");
			break;
		}
	} while (choice != 'r' && choice != 'R');

	// 释放链表空间
	List_Destroy(list, schedule_node_t);

	return;
}


//
//标识符：TTMS_SCU_Schedule_UI_Add
//函数声明：int Schedule_UI_Add(int play_id);
//函数功能：接收键盘输入的演出计划信息，通过调用业务逻辑层函数来添加新演出计划。
//参数说明：play_id为整型，表示与演出计划相关的剧目ID号。
//返回值：整型，返回1表示添加新演出计划成功，返回0表示添加新演出计划失败。
//
int Schedule_UI_Add(int play_id)
{

	int newcount = 0;
	
		printf("\n=======================================================\n");
		printf("****************  Add New Show Schedule  ****************\n");
		printf("-------------------------------------------------------\n");
		schedule_t new;
		scanf(" %*c"); // 吸收前导换行
		printf("The new Schedule id: ");
		scanf("%d", &new.id); // 读取剧目ID
		scanf(" %*c"); // 吸收前导换行
		printf("The new Schedule name: ");
		scanf("%[^\n]", new.name); // 读取剧目名称
		scanf(" %*c"); // 吸收前导换行
		printf("The new Schedule's studio room: ");
		scanf("%[^\n]", new.studio); // 读取演出厅名称
		printf("The new Schedule's start data:\tyear-month-day ");
		scanf("%d-%d-%d", &new.startdate.year, &new.startdate.month, &new.startdate.day); // 读取开始日期
		printf("The new Schedule's start time:\thour:minute ");
		scanf("%d:%d", &new.starttime.hour, &new.starttime.minute); // 读取开始时间
		printf("The new Schedule's start data:\tyear-month-day ");
		scanf("%d-%d-%d", &new.enddate.year, &new.enddate.month, &new.enddate.day); // 读取结束日期
		printf("The new Schedule's start time:\thour:minute ");
		scanf("%d:%d", &new.endtime.hour, &new.endtime.minute); // 读取结束时间

		if (Schedule_Srv_Add(&new)) {
			printf("New schedule added successfully!\n");
			newcount=1;
		}
		else {
			printf("Failed to add new schedule.\n");
		}
		

	

	return newcount;
}

//
//标识符：TTMS_SCU_Schedule_UI_Mod
//函数声明：int Schedule_UI_Modify(int id);
//函数功能：根据演出计划id的值，调用业务逻辑层函数修改演出计划数据
//参数说明：id为整型，表示待修改演出计划的ID号。
//返回值：整型，返回1表示添加新演出计划成功，返回0表示添加新演出计划失败。
//
int Schedule_UI_Modify(int id)
{
	schedule_list_t list;
	schedule_node_t* pos;
	schedule_t rec;
	int found = 0;
	int rtn = 0;

	List_Init(list, schedule_node_t);

	/* 载入所有演出计划 */
	if (Schedule_Srv_FetchAll(list) <= 0) {
		printf("No schedules in system!\n");
		List_Destroy(list, schedule_node_t);
		return 0;
	}

	/* 在链表中查找要修改的记录 */
	List_ForEach(list, pos) {
		if (pos->data.id == id) {
			found = 1;
			rec = pos->data; /* 拷贝当前记录以便修改 */
			break;
		}
	}

	if (!found) {
		printf("The schedule does not exist!\nPress [Enter] to return!\n");
		getchar();
		List_Destroy(list, schedule_node_t);
		return 0;
	}

	/* 显示当前值并提示修改（按需覆盖） */
	printf("\n=======================================================\n");
	printf("****************  Update Schedule  ****************\n");
	printf("Schedule ID: %d\n", rec.id);
	printf("Play Name: %s\n", rec.name);

	printf("Studio [%s]: ", rec.studio);
	/* 使用空格前导使 scanf 跳过残留换行；如果只想保留旧值，输入一个换行不可行 */
	scanf(" %29[^\n]", rec.studio);

	printf("Start Date [%04d-%02d-%02d] (YYYY MM DD): ",
		rec.startdate.year, rec.startdate.month, rec.startdate.day);
	scanf("%d-%d-%d", &rec.startdate.year, &rec.startdate.month, &rec.startdate.day);
	printf("Start Time [%02d:%02d] (HH MM): ",
		rec.starttime.hour, rec.starttime.minute);
	scanf("%d:%d", &rec.starttime.hour, &rec.starttime.minute);

	printf("End Date [%04d-%02d-%02d] (YYYY MM DD): ",
		rec.enddate.year, rec.enddate.month, rec.enddate.day);
	scanf("%d-%d-%d", &rec.enddate.year, &rec.enddate.month, &rec.enddate.day);
	printf("End Time [%02d:%02d] (HH MM): ",
		rec.endtime.hour, rec.endtime.minute);
	scanf("%d:%d", &rec.endtime.hour, &rec.endtime.minute);

	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		printf("The schedule data updated successfully!\nPress [Enter] key to return!\n");
		rtn = 1;
	}
	else {
		printf("The schedule data update failed!\nPress [Enter] key to return!\n");
		rtn = 0;
	}

	getchar();
	List_Destroy(list, schedule_node_t);
	return rtn;
}

//标识符：TTMS_SCU_Schedule_UI_Del
//函数声明：int Schedule_UI_Delete(int id);
//函数功能：根据演出计划的id值，调用业务逻辑层函数删除演出计划
//参数说明：id为整型，表示待删除的演出计划ID号。
//返回值：整型，返回1表示删除演出计划成功，非1表示删除演出计划失败。
int Schedule_UI_Delete(int id)
{
	int rtn = 0;
	if (Schedule_Srv_DeleteByID(id) != 1) {
		printf("Failed to delete schedule!\n");
		rtn = 0;
	}
	else {
		printf("Schedule deleted successfully!\n");
		rtn = 1;
	}
	return rtn;
}

//标识符：TTMS_SCU_Schedule_UI_List
//函数声明：void Schedule_UI_ListAll(void);
//函数功能：根据演出计划的id值，调用业务逻辑层函数删除演出计划
//参数说明：无
//返回值：无
void Schedule_UI_ListAll(void) {
	schedule_list_t list;
	schedule_node_t* pos;
	int i = 0;

	List_Init(list, schedule_node_t);

	if (Schedule_Srv_FetchAll(list) <= 0) {
		printf("No schedules in system!\n");
		List_Destroy(list, schedule_node_t);
		return;
	}

	printf("\n==================================================================\n");
	printf("********************** Schedule List **********************\n");
	printf("%5s  %20s  %12s  %20s\n", "ID", "Play Name", "Studio", "Start - End");
	printf("------------------------------------------------------------------\n");

	List_ForEach(list, pos) {
		printf("%5d  %20s  %12s  %04d-%02d-%02d %02d:%02d  -  %04d-%02d-%02d %02d:%02d\n",
			pos->data.id, pos->data.name, pos->data.studio,
			pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
			pos->data.starttime.hour, pos->data.starttime.minute,
			pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
			pos->data.endtime.hour, pos->data.endtime.minute);
		i++;
	}

	printf("------------------------------------------------------------------\n");
	printf("Total records: %d\n", i);
	printf("==================================================================\n");

	List_Destroy(list, schedule_node_t);
}

//标识符：TTMS_SCU_Schedule_UI_Qry
//函数声明：int Schedule_UI_Query(char* play_name);
//函数功能：显示与剧目相关联的演出计划的信息
//参数说明：无
//返回值：整型，获取到的演出计划的数量
int Schedule_UI_Query(char* play_name)
{
	schedule_list_t list;
	schedule_node_t* pos;
	int cnt = 0;

	if (play_name == NULL || strlen(play_name) == 0) {
		return 0;
	}

	List_Init(list, schedule_node_t);

	if (Schedule_Srv_FetchAll(list) <= 0) {
		List_Destroy(list, schedule_node_t);
		return 0;
	}

	printf("\n==================================================================\n");
	printf("********************** Schedule Query Result **********************\n");
	printf("%5s  %20s  %12s  %20s\n", "ID", "Play Name", "Studio", "Start - End");
	printf("------------------------------------------------------------------\n");

	List_ForEach(list, pos) {
		if (strstr(pos->data.name, play_name) != NULL) {
			printf("%5d  %20s  %12s  %04d-%02d-%02d %02d:%02d  -  %04d-%02d-%02d %02d:%02d\n",
				pos->data.id, pos->data.name, pos->data.studio,
				pos->data.startdate.year, pos->data.startdate.month, pos->data.startdate.day,
				pos->data.starttime.hour, pos->data.starttime.minute,
				pos->data.enddate.year, pos->data.enddate.month, pos->data.enddate.day,
				pos->data.endtime.hour, pos->data.endtime.minute);
			cnt++;
		}
	}

	printf("------------------------------------------------------------------\n");
	printf("Matched records: %d\n", cnt);
	printf("==================================================================\n");

	List_Destroy(list, schedule_node_t);  
	return cnt;
}

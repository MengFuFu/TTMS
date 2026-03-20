#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <string.h>
accont_t gl_CurUser;
static const int SALESANALYSIS_PAGE_SIZE = 5;

//降序显示票房排行榜
void SalesAnalysis_UI_BoxOffice() {
	;//?
}

//显示售票员在给定日期间的售票情况
void SalesAnalysis_UI_StaSale(int userID, user_date_t stDate, user_date_t endDate) {
	;//?
}

void SalesAnalysis_UI_MgtEn() {
	int i, id;
	char choice;
	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;
	List_lnit(head,salesanalysis_node_t);
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		system("cls");
		printf("\n=============================================================================================================\n");
		printf("                                             票房排行榜                                                        \n");
		printf("%5s %18s %18s %10s %10s %10s %10s %9s %9s\n","剧目ID","剧目名称","剧目区域","剧目时长","剧目上座数量","剧目票房","剧目票价","剧目上映日期","剧目下映日期");
		printf("---------------------------------------------------------------------------------------------------------------\n");
		for (i = 0, pos = salesanalysis_node_t * paging.curPos; pos != head && i < paging.pageSize;i++) {
			printf("%5d %18s %18s %10d %10d %10d %10d %5d-%2d-%2d %5d-%2d-%2d\n",pos->data.play_id, pos->data.name, pos->data.area, pos->data.duration, pos->data.totaltickets,pos->data.sales, pos->data.price, pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day, pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t-------------------共%2d项--------------------第%2d/%2d页-------------------------------------------\n",paging.totalRecords,Pageing_CurPage(paging), Pageing_TotalPage(paging));
		printf("请输入你的选择：");
		fflush(stdin);
		switch (choice) {
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
			}
			break;
		}
		printf("请输入任意键继续...");
		getchar(); getchar();
	} while (choice != "r" && choice != "R");
}
List_Destory(head, salesanalysis_node_t);
//统计个人销售额界面
void StaSales_UI_Self() {
	int id = gl_CurUser.id;
	user_date_t curdate, startdate, enddate;//在common.h里
	curdate = DateNow();//在common.h里
	startdate.year = curdate.year;
	startdate.month = curdate.month;
	startdate.day = 1;
	enddate.year = curdate.year;
	enddate.month = curdate.month;
	enddate.day = 31;
	int moneyByMySelf=0;
	char choice;
	do {
		system("cls");
		printf("\n============================================================================================================\n");
		printf("1.查询当日的销售统计");
		printf("2.查询当月的销售统计");
	    printf("---------------------------------------------------------------------------------------------------\n");
		printf("***************************************************************************************************\n");
		printf("\n=================================================================================================\n");
		printf("请输入你的选择");
		//？
		fflush(stdin);
		scanf_s("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case '1'://查日
			moneyByMySelf = StaSales_Srv_CompSaleVal(id,startdate,enddate);//在SalesAnalysis里
			printf("\t\t\t您今天的销售额为：%d\n", moneyByMySelf);
			break;
		case '2'://查月
			;
			break;
			printf("按任意键继续...");
			getchar(); getchar();
		}
	} while (choice != '0');
}
//统计售票员销售额界面
void StaSales_UI_Clerk() {
	int id, money;
	char Username[33];
	user_date_t startdate, enddate;//在common.h里
	account_t user;//?
	printf("\t\t\t请输入售票员姓名：");
	safeInputStringInternal(* Username, 33);
	if (Account_Srv_FetchByName(Username, &user)&&user.type==USR_CLERK) {
		id = user.id;
		printf("\t\t\t请输入起始时间(年 月 日):");
		scanf_s("%d %d %d", &startdata.year, &startdata.month, &startdata.day);
		printf("\t\t\t请输入结束时间(年 月 日):");
		scanf_s("%d %d %d", &enddata.year, &enddata.month, &enddata.day);
		money= StaSales_Srv_CompSaleVal(id, startdate, enddate);//在SalesAnalysis.c里
		if (money == -1) {
			printf("\t\t\t目前还没有销售记录...\n");
			return;
		}
	}
	else if (!Account_Srv_FetchByName(Username, &user)) {
		printf("\t\t\t该用户名信息不存在...\n");
		return;
	}
	else if (user.type != USR_CLERK) {
		printf("\t\t\t该用户不是售票员...\n");
		return;
	}
	printf("\t\t\t售票员：%s的销售额为：%d\n",Username,money);
	return;
}
//统计销售额界面
void StaSales_UI_MgtEnt(account_CurUser) {
	strcpy(gl_CurUser.username, CurUser.username);
	strcpy(gl_CurUser.password, CurUser.password);
	gl_CurUser.id = CurUser.id;
	gl_CurUser.type = CurUser.type;
	printf("%s----%s\n", gl_CurUser.username, CurUser.username);
	getchar();
	getchar();//暂停，等待下一步
	char choice;
	do {
		system("cls");
		printf("\n===========================================================================================================\n");
		printf("1.统计售票员销售额...\n");
		printf("2.统计个人销售额...\n");
		printf("---------------------------------------------------------------------------------------------------\n");
		printf("***************************************************************************************************\n");
		printf("\n=================================================================================================\n");
		printf("请输入你的选择");
		fflush(stdin);
		scanf_s("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case '1':
			if (gl_CurUser.type == USR_MANG) {
				StaSales_UI_Clerk();
			}
			else {
				printf("\t\t\t没有此权限...");
			}
			break;
		case '2':
			if (gl_CurUser.type == USR_CLERK) {
				StaSales_UI_Self();
			}
			else {
				printf("\t\t\t没有此权限...");
			}
			break;
			printf("按任意键继续...");
			getchar(); getchar();
		}
	} while (choice != "r" && choice != "R");
	//释放链表空间
	
}
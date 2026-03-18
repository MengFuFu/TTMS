/*
* Copyright(C), 2007-2008, XUPT Univ.	
* 用例编号：TTMS_UC_01	 
* File name: Studio_UI.c			  
* Description : 演出厅用例界面层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/
#define _CRT_SECURE_NO_WARNINGS
#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../SafeStdin/safe_stdin.h"


static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

/*
标识符：TTMS_SCU_Studio_UI_MgtEnt 
函数功能：界面层管理演出厅的入口函数，显示当前的演出厅数据，并提供演出厅数据添加、修改及删除功能操作的入口。
参数说明：无。
返 回 值：无。
*/
void Studio_UI_MgtEntry(void) {
    int i, id;
    char choice[10];

    studio_list_t head;
    studio_node_t* pos;
    Pagination_t paging;

    List_Init(head, studio_node_t);
    paging.offset = 0;
    paging.pageSize = STUDIO_PAGE_SIZE;

    //载入数据
    paging.totalRecords = Studio_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head, paging);

    do {
        // 统一分隔线长度，和表格宽度完全匹配
        printf("\n====================================================================================================\n");
        printf("******************************  Projection Room List  ******************************\n");
        // 【核心优化】表头和内容列宽完全对应，左对齐+右对齐搭配，零错位
        printf("%5s  %-24s  %12s  %12s  %12s\n",
            "ID", "Room Name", "Rows Count", "Columns Count", "Seats Count");
        printf("----------------------------------------------------------------------------------------------------\n");

        // 显示数据，和表头列宽100%匹配
        Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i) {
            printf("%5d  %-24s  %12d  %12d  %12d\n",
                pos->data.id,
                pos->data.name,
                pos->data.rowsCount,
                pos->data.colsCount,
                pos->data.seatsCount);
        }

        printf("----------------------------------------------------------------------------------------------------\n");
        printf("------- Total Records:%2d  ---------------------------------------  Page %2d/%2d  -------\n",
            paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("****************************************************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]eat | [R]eturn\n");
        printf("====================================================================================================\n");

        readString(choice, sizeof(choice), "Your Choice:");

        switch (choice[0]) {
        case 'a':
        case 'A':
            if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
            {
                paging.totalRecords = Studio_Srv_FetchAll(head);
                Paging_Locate_LastPage(head, paging, studio_node_t);
            }
            break;
        case 'd':
        case 'D':
            id = readInt("Input the ID:");
            if (Studio_UI_Delete(id)) {	//从新载入数据
                paging.totalRecords = Studio_Srv_FetchAll(head);
                List_Paging(head, paging, studio_node_t);
            }
            break;
        case 'u':
        case 'U':
            id = readInt("Input the ID:");
            if (Studio_UI_Modify(id)) {	//从新载入数据
                paging.totalRecords = Studio_Srv_FetchAll(head);
                List_Paging(head, paging, studio_node_t);
            }
            break;
        case 's':
        case 'S':
            id = readInt("Input the ID:");
            //Seat_UI_MgtEntry(id);
            paging.totalRecords = Studio_Srv_FetchAll(head);
            List_Paging(head, paging, studio_node_t);
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
            }
            break;
        }
    } while (choice[0] != 'r' && choice[0] != 'R');
    //释放链表空间
    List_Destroy(head, studio_node_t);
}
/*
标识符：TTMS_SCU_Studio_UI_Add 
函数功能：用于向系统中添加一个新演出厅数据。
参数说明：无。
返 回 值：整型，成功添加新演出厅的个数。
*/
int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice[10]; 

	do {
		printf("\n=======================================================\n");
		printf("****************  Add New Projection Room  ****************\n");
		printf("-------------------------------------------------------\n");

		readString(rec.name, sizeof(rec.name), "Room Name:");
		rec.rowsCount = readInt("Row Count of Seats:");
		rec.colsCount = readInt("Column Count of Seats:");

		rec.seatsCount = 0;
		printf("=======================================================\n");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("The new room added successfully!\n");
		}
		else {
			printf("The new room added failed!\n");
		}
		printf("-------------------------------------------------------\n");

		readString(choice, sizeof(choice), "[A]dd more, [R]eturn:");
	} while (choice[0] == 'a' || choice[0] == 'A');

	return newRecCount;
}

/*
标识符：TTMS_SCU_Studio_UI_Mod
函数功能：用于修改系统中现存的一个演出厅数据。
参数说明：id为整型，是需要修改的演出厅ID。
返 回 值：整型，表示是否成功修改了演出厅的标志。
*/
int Studio_UI_Modify(int id) {
    studio_t rec;
    int rtn = 0;
    int newrow, newcolumn;
    seat_list_t list;
    int seatcount;
    char tempInput[10];

    /*Load record*/
    if (!Studio_Srv_FetchByID(id, &rec)) {
        printf("The room does not exist!\nPress [Enter] key to return!\n");
        readString(tempInput, sizeof(tempInput), "");
        return 0;
    }

    printf("\n=======================================================\n");
    printf("****************  Update Projection Room  ****************\n");
    printf("-------------------------------------------------------\n");
    printf("Room ID:%d\n", rec.id);
    printf("Room Name[%s]:", rec.name);

    readString(rec.name, sizeof(rec.name), "");

    List_Init(list, seat_node_t);
    seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
    if (seatcount) {
        do {
            //如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
            printf("Row Count of Seats should >= [%d]:", rec.rowsCount);
            newrow = readInt("");
            printf("Column Count of Seats should >= [%d]:", rec.colsCount);
            newcolumn = readInt("");
        } while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
        rec.rowsCount = newrow;
        rec.colsCount = newcolumn;
        rec.seatsCount = seatcount;
    }
    else {
        rec.rowsCount = readInt("Row Count of Seats:");
        rec.colsCount = readInt("Column Count of Seats:");
        rec.seatsCount = 0;
    }

    printf("-------------------------------------------------------\n");

    if (Studio_Srv_Modify(&rec)) {
        rtn = 1;
        printf(
            "The room data updated successfully!\nPress [R / r] key to return!\n");
    }
    else {
        printf("The room data updated failed!\nPress [R / r] key to return!\n");
    }

    readString(tempInput, sizeof(tempInput), "");
    return rtn;
}

/*
标识符：TTMS_SCU_Studio_UI_Del
函数功能：用于删除系统中现存的一个演出厅数据。
参数说明：id为整型，是需要删除的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅的标志。
*/

int Studio_UI_Delete(int id) {
    int rtn = 0;
    char tempInput[10];

    if (Studio_Srv_DeleteByID(id)) {
        //在删除放映厅时，同时根据放映厅id删除座位文件中的座位
        if (Seat_Srv_DeleteAllByRoomID(id))
            printf("The seats of the room deleted successfully!\n");
        printf(
            "The room deleted successfully!\nPress [R / r] key to return!\n");
        rtn = 1;
    }
    else {
        printf("The room does not exist!\nPress [R / r] key to return!\n");
    }

    readString(tempInput, sizeof(tempInput), "");
    return rtn;
}
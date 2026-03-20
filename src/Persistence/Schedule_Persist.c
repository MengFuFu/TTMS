#define _CRT_SECURE_NO_WARNINGS

//
//Copyright(C), 2007-2008, XUPT Univ.
//用例编号：TTMS_UC_04	TTMS_UC_06
//File name: Schedule_UI.h
//Description : 演出计划业务界面层头文件
//Author:   XUPT
//Version:  v.1
//Date: 	2015年4月22日
//


#include "Schedule_Persist.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../Common/List.h"

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";

//标识符：TTMS_SCU_Schedule_Perst_SelByPlay
//函数声明：int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);
//函数功能：从演出计划数据文件中载入与指定剧目关联的演出计划信息，构成链表list
//参数说明：list为schedule_t类型，表示符合条件的演出计划信息链表头指针；play_id为整型，表示指定的剧目ID号。
//返回值：整型，表示载入到链表list中的演出计划的数量。
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;
    char playName[64] = {0};

    assert(NULL!=list);

    // 清空目标链表
    List_Free(list, schedule_node_t);

    // 先尝试从剧目文件中根据 play_id 获取 play 名称（schedule.name 存储的是剧目名称）
    FILE *fpPlay = fopen("Play.dat", "rb");
    if (fpPlay) {
        play_t p;
        while (!feof(fpPlay)) {
            if (fread(&p, sizeof(play_t), 1, fpPlay)) {
                if (p.id == play_id) {
                    strncpy(playName, p.name, sizeof(playName)-1);
                    break;
                }
            }
        }
        fclose(fpPlay);
    }

    // 若未找到剧目名，直接返回 0
    if (playName[0] == '\0') {
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) { // 文件不存在
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            // 通过剧目名称匹配（schedule.name 存放剧目名称）
            if (strcmp(data.name, playName) == 0) {
                newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
                if (!newNode) {
                    printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                    break;
                }
                newNode->data = data;
                List_AddTail(list, newNode);
                recCount++;
            }
        }
    }

    fclose(fp);
    return recCount;
}

//标识符：TTMS_SCU_Schedule_Perst_Insert
//函数声明：int Schedule_Perst_Insert(schedule_t* data);
//函数功能：向演出计划文件Schedule.dat中添加一条新的演出计划。
//参数说明：data为schedule_t类型指针，指向待存储的新演出计划数据。
//返回值：
int Schedule_Perst_Insert(schedule_t* data)
{
    assert(NULL!=data);

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
    int rtn = 0;
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
        return 0;
    }

    rtn = fwrite(data, sizeof(schedule_t), 1, fp);

    fclose(fp);
    return rtn;
}

//标识符：TTMS_SCU_Schedule_Perst_Mod
//函数声明：int Schedule_Perst_Update(const schedule_t* data);
//函数功能：更新演出计划数据文件Schedule.dat中的一条演出信息
//参数说明：data为schedule_t类型指针，存放新的演出计划数据。
//返回值：整型，符号1表示更新演出计划成功，否则更新演出计划失败。
int Schedule_Perst_Update(const schedule_t* data)
{
    assert(NULL!=data);

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
        return 0;
    }

    schedule_t buf;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&buf, sizeof(schedule_t), 1, fp)) {
            if (buf.id == data->id) {
                fseek(fp, -((int)sizeof(schedule_t)), SEEK_CUR);
                fwrite(data, sizeof(schedule_t), 1, fp);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found;
}

//标识符：TTMS_SCU_Schedule_Perst_RemByID
//函数声明：int Schedule_Perst_DeleteByID(int id);
//函数功能：去除演出计划文件Schedule.dat中指定ID的演出计划。
//参数说明：id为整型，表示待删除的演出计划ID号。
//返回值：整型，返回1表示删除演出计划成功，否则删除演出计划失败。
int Schedule_Perst_DeleteByID(int id)
{
    // 将原文件重命名为临时文件，然后将不等于 id 的记录写回新文件
    if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0) {
        // 若重命名失败（文件可能不存在），则视为失败
        return 0;
    }

    FILE *fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
    if (NULL == fpSour) {
        return 0;
    }

    FILE *fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
    if (NULL == fpTarg) {
        fclose(fpSour);
        return 0;
    }

    schedule_t data;
    int found = 0;
    while (!feof(fpSour)) {
        if (fread(&data, sizeof(schedule_t), 1, fpSour)) {
            if (data.id == id) {
                found = 1;
                continue; // 跳过要删除的记录
            }
            fwrite(&data, sizeof(schedule_t), 1, fpTarg);
        }
    }

    fclose(fpTarg);
    fclose(fpSour);

    remove(SCHEDULE_DATA_TEMP_FILE);
    return found;
}

//标识符：TTMS_SCU_Schedule_Perst_SelAll
//函数声明：int Schedule_Perst_SelectAll(schedule_list_t list);
//函数功能：从演出计划数据文件Schedule.dat中载入全部演出计划信息，构成链表list。
//参数说明：list为schedule_list_t类型，表示演出计划信息链表头指针。
//返回值：整型，表示载入到链表list中的演出计划的数量。
int Schedule_Perst_SelectAll(schedule_list_t list)
{
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;

    assert(NULL!=list);

    List_Free(list, schedule_node_t);

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) { // 文件不存在
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = data;
            List_AddTail(list, newNode);
            recCount++;
        }
    }

    fclose(fp);
    return recCount;
}

//标识符：TTMS_SCU_Schedule_Perst_SelByName
//函数声明：int Schedule_Perst_SelectByName(play_list_t list,  char condt[]);
//函数功能：从剧目信息文件Play.dat中载入剧目名称包含指定字符串的剧目信息。
//参数说明：list为play_t类型，表示载入的剧目信息的链表头指针；condt为字符数组，表示指定的字符串。
//返回值：整型，表示载入到链表list中的演出计划的数量。
int Schedule_Perst_SelectByName(play_list_t list, char condt[])
{
    play_node_t *newNode;
    play_t data;
    int recCount = 0;

    assert(NULL!=list);

    List_Free(list, play_node_t);

    FILE *fp = fopen("Play.dat", "rb");
    if (NULL == fp) {
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(play_t), 1, fp)) {
            if (condt != NULL && strstr(data.name, condt) != NULL) {
                newNode = (play_node_t*) malloc(sizeof(play_node_t));
                if (!newNode) {
                    printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                    break;
                }
                newNode->data = data;
                List_AddTail(list, newNode);
                recCount++;
            }
        }
    }

    fclose(fp);
    return recCount;
}


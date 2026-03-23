#define _CRT_SECURE_NO_WARNINGS

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

int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;
    char playName[64] = {0};

    assert(NULL!=list);

    List_Free(list, schedule_node_t);

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

    if (playName[0] == '\0') {
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            if (strcmp(data.name, playName) == 0) {
                newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
                if (!newNode) {
                    printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                    break;
                }
                newNode->data = data;
                newNode->next = newNode->prev = newNode;
                List_AddTail(list, newNode);
                recCount++;
            }
        }
    }

    fclose(fp);
    return recCount;
}

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

int Schedule_Perst_DeleteByID(int id)
{
    if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0) {
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
                continue;
            }
            fwrite(&data, sizeof(schedule_t), 1, fpTarg);
        }
    }

    fclose(fpTarg);
    fclose(fpSour);

    remove(SCHEDULE_DATA_TEMP_FILE);
    return found;
}

int Schedule_Perst_SelectAll(schedule_list_t list)
{
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;

    assert(NULL!=list);

    List_Free(list, schedule_node_t);

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) {
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
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            recCount++;
        }
    }

    fclose(fp);
    return recCount;
}

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
                newNode->next = newNode->prev = newNode;
                List_AddTail(list, newNode);
                recCount++;
            }
        }
    }

    fclose(fp);
    return recCount;
}

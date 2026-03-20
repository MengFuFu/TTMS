/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_08
* File name: Sale_Persist.c
* Description : 售票持久层实现
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Sale_Persist.h"
#include "EntityKey_Persist.h"
#include "Ticket_Persist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SALE_DATA_FILE "Sale.dat"
#define TICKET_DATA_FILE "Ticket.dat"
#define SCHEDULE_DATA_FILE "Schedule.dat"

int Sale_Perst_Insert(const sale_t *data) {
    if (!data) return 0;

    int rtn = 0;
    FILE *fp = NULL;
    int id = EntKey_Perst_GetNewKeys("sale", 1);
    if (id <= 0) {
        return rtn;
    }

    fp = fopen(SALE_DATA_FILE, "ab");
    if (fp == NULL) {
        return rtn;
    }

    sale_t sale = *data;
    sale.id = id;
    if (fwrite(&sale, sizeof(sale_t), 1, fp) == 1) {
        rtn = id;
    }

    fclose(fp);
    return rtn;
}

int Sale_Perst_DeleteByID(int id) {
    if (id <= 0) return 0;

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    FILE *tmp = fopen("sale_tmp.dat", "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    sale_t buf;
    int found = 0;
    while (fread(&buf, sizeof(sale_t), 1, fp) == 1) {
        if (buf.id != id) {
            fwrite(&buf, sizeof(sale_t), 1, tmp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(SALE_DATA_FILE);
    rename("sale_tmp.dat", SALE_DATA_FILE);
    return found;
}

int Sale_Perst_SelectByID(int id, sale_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(sale_t));

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    if (!fp) return 0;

    sale_t data;
    int found = 0;
    while (fread(&data, sizeof(sale_t), 1, fp) == 1) {
        if (data.id == id) {
            *buf = data;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

int Sale_Perst_SelectAll(sale_list_t list) {
    if (!list) return 0;

    FILE *fp = fopen(SALE_DATA_FILE, "rb");
    if (!fp) return 0;

    sale_t data;
    int count = 0;
    sale_node_t *newNode;

    while (fread(&data, sizeof(sale_t), 1, fp) == 1) {
        newNode = (sale_node_t *)malloc(sizeof(sale_node_t));
        if (newNode) {
            memset(newNode, 0, sizeof(sale_node_t));
            newNode->data = data;
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            count++;
        }
    }
    fclose(fp);
    return count;
}

int Ticket_Perst_SelectBySchID(int id, ticket_list_t list) {
    if (!list || id <= 0) return 0;

    FILE *fp = fopen(TICKET_DATA_FILE, "rb");
    if (!fp) return 0;

    ticket_t data;
    int count = 0;
    ticket_node_t *node;

    while (fread(&data, sizeof(ticket_t), 1, fp) == 1) {
        if (data.schedule_id == id) {
            node = (ticket_node_t *)malloc(sizeof(ticket_node_t));
            if (node) {
                memset(node, 0, sizeof(ticket_node_t));
                node->data = data;
                node->next = node->prev = node;
                List_AddTail(list, node);
                count++;
            }
        }
    }
    fclose(fp);
    return count;
}

ticket_node_t* Ticket_Perst_SelectBySeatID(ticket_list_t list, int seat_id) {
    if (!list || seat_id <= 0) return NULL;

    ticket_node_t *pos;
    List_ForEach(list, pos) {
        if (pos && pos->data.seat_id == seat_id) {
            return pos;
        }
    }
    return NULL;
}

int Schedule_Perst_Insert(const schedule_t *data) {
    if (!data) return 0;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
    if (!fp) return 0;

    int rtn = fwrite(data, sizeof(schedule_t), 1, fp);
    fclose(fp);
    return rtn;
}

int Schedule_Perst_Update(const schedule_t *data) {
    if (!data || data->id <= 0) return 0;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
    if (!fp) return 0;

    schedule_t rec;
    int found = 0;
    while (fread(&rec, sizeof(schedule_t), 1, fp) == 1) {
        if (rec.id == data->id) {
            fseek(fp, -(long)sizeof(schedule_t), SEEK_CUR);
            if (fwrite(data, sizeof(schedule_t), 1, fp) == 1) {
                found = 1;
            }
            break;
        }
    }
    fclose(fp);
    return found;
}

int Schedule_Perst_DeleteByID(int id) {
    if (id <= 0) return 0;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    FILE *tmp = fopen("schedule_tmp.dat", "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    schedule_t buf;
    int found = 0;
    while (fread(&buf, sizeof(schedule_t), 1, fp) == 1) {
        if (buf.id != id) {
            fwrite(&buf, sizeof(schedule_t), 1, tmp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(SCHEDULE_DATA_FILE);
    rename("schedule_tmp.dat", SCHEDULE_DATA_FILE);
    return found;
}

int Schedule_Perst_SelectByID(int id, schedule_t *buf) {
    if (!buf || id <= 0) return 0;
    memset(buf, 0, sizeof(/*schedule_t*/buf));

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (!fp) return 0;

    schedule_t data;
    int found = 0;
    while (fread(&data, sizeof(schedule_t), 1, fp) == 1) {
        if (data.id == id) {
            *buf = data;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

int Schedule_Perst_SelectByPlayID(schedule_list_t list, int playID) {
    if (!list || playID <= 0) return 0;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (!fp) return 0;

    schedule_t data;
    int count = 0;
    schedule_node_t *node;

    while (fread(&data, sizeof(schedule_t), 1, fp) == 1) {
        if (data.play_id == playID) {
            node = (schedule_node_t *)malloc(sizeof(schedule_node_t));
            if (node) {
                memset(node, 0, sizeof(schedule_node_t));
                node->data = data;
                node->next = node->prev = node;
                List_AddTail(list, node);
                count++;
            }
        }
    }
    fclose(fp);
    return count;
}

int Schedule_Perst_SelectAll(schedule_list_t list) {
    if (!list) return 0;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (!fp) return 0;

    schedule_t data;
    int count = 0;
    schedule_node_t *node;

    while (fread(&data, sizeof(schedule_t), 1, fp) == 1) {
        node = (schedule_node_t *)malloc(sizeof(schedule_node_t));
        if (node) {
            memset(node, 0, sizeof(schedule_node_t));
            node->data = data;
            node->next = node->prev = node;
            List_AddTail(list, node);
            count++;
        }
    }
    fclose(fp);
    return count;
}

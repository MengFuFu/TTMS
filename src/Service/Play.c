/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Play.c
* Description: Play business logic implementation
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#include "Play.h"
#include "../Common/List.h"
#include "../Persistence/Play_Persist.h"
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

int Play_Srv_Add(play_t* data) {
    if (!data) return 0;

    play_list_t tempList;
    List_Init(tempList, play_node_t);
    Play_Perst_SelAll(tempList);

    int maxID = 0;
    play_node_t* pos;
    List_ForEach(tempList, pos) {
        if (pos && pos->data.id > maxID && pos->data.id != 0xCCCCCCCC) {
            maxID = pos->data.id;
        }
    }
    data->id = maxID + 1;

    List_Destroy(tempList, play_node_t);
    return Play_Perst_Insert(data);
}

int Play_Srv_Modify(play_t* data) {
    return Play_Perst_Update(data);
}

int Play_Srv_DeleteByID(int ID) {
    return Play_Perst_DeleteByID(ID);
}

int Play_Srv_FetchAll(play_list_t list) {
    return Play_Perst_SelAll(list);
}

int Play_Srv_FetchByID(int id, play_t* buf) {
    if (!buf) return 0;
    memset(buf, 0, sizeof(play_t));

    play_list_t list;
    List_Init(list, play_node_t);
    Play_Perst_SelAll(list);

    int found = 0;
    play_node_t* pos;
    List_ForEach(list, pos) {
        if (pos && pos->data.id == id) {
            *buf = pos->data;
            found = 1;
            break;
        }
    }

    List_Destroy(list, play_node_t);
    return found;
}

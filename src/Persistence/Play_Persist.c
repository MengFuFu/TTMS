#include "../Persistence/Play_Persist.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLAY_DATA_FILE "play.dat"

// 载入全部剧目（对应PPT：TTMS_SCU_Play_Perst_SelAll）
int Play_Perst_SelAll(play_list_t list) {
    if (!list) return 0;

    FILE* fp = fopen(PLAY_DATA_FILE, "rb");
    if (!fp) return 0;

    play_t data;
    int count = 0;
    play_node_t* newNode;

    while (fread(&data, sizeof(play_t), 1, fp) == 1) {
        newNode = (play_node_t*)malloc(sizeof(play_node_t));
        if (newNode) {
            // ✅ 清0堆内存，避免调试值泄露
            memset(newNode, 0, sizeof(play_node_t));
            newNode->data = data;
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            count++;
        }
    }
    fclose(fp);
    return count;
}

// 插入新剧目
int Play_Perst_Insert(play_t* data) {
    if (!data) return 0;

    FILE* fp = fopen(PLAY_DATA_FILE, "ab");
    if (!fp) return 0;
    int rtn = fwrite(data, sizeof(play_t), 1, fp);
    fclose(fp);
    return rtn;
}

// 更新剧目
int Play_Perst_Update(play_t* data) {
    if (!data) return 0;

    FILE* fp = fopen(PLAY_DATA_FILE, "rb+");
    if (!fp) return 0;

    play_t buf;
    int found = 0;
    while (fread(&buf, sizeof(play_t), 1, fp) == 1) {
        if (buf.id == data->id) {
            fseek(fp, -(long)sizeof(play_t), SEEK_CUR);
            fwrite(data, sizeof(play_t), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

// 根据ID删除剧目
int Play_Perst_DeleteByID(int id) {
    FILE* fp = fopen(PLAY_DATA_FILE, "rb");
    FILE* tmp = fopen("play_tmp.dat", "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        return 0;
    }

    play_t buf;
    int found = 0;
    while (fread(&buf, sizeof(play_t), 1, fp) == 1) {
        if (buf.id != id) {
            fwrite(&buf, sizeof(play_t), 1, tmp);
        }
        else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(PLAY_DATA_FILE);
    rename("play_tmp.dat", PLAY_DATA_FILE);
    return found;
}
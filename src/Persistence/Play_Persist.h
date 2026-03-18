#ifndef PLAY_PERST_H
#define PLAY_PERST_H

#include "../Service/Play.h"

// -------------------------- 持久化层函数声明 --------------------------
int  Play_Perst_SelAll(play_list_t list);
int  Play_Perst_Insert(play_t* data);
int  Play_Perst_Update(play_t* data);
int  Play_Perst_DeleteByID(int id);

#endif
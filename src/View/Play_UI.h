#ifndef PLAY_UI_H
#define PLAY_UI_H

#include "../Service/Play.h"
#include "../SafeStdin/safe_stdin.h"

void Play_UI_MgtEntry(void);
int  Play_UI_Add(void);
int  Play_UI_Modify(int id);
int  Play_UI_Delete(int id);
void Play_UI_Query(play_list_t list);

#endif

#ifndef PLAY_UI_H
#define PLAY_UI_H

#include "../Service/Play.h"
#include "../SafeStdin/safe_stdin.h"  // 你的项目安全输入头文件

// -------------------------- 界面层函数声明（对应PPT） --------------------------
void Play_UI_MgtEntry(void);  // 管理剧目界面（对应TTMS_SCU_Play_UI_MgtEnt）
int  Play_UI_Add(void);       // 添加新剧目界面
int  Play_UI_Modify(int id);  // 修改剧目界面
int  Play_UI_Delete(int id);  // 删除剧目界面
void Play_UI_Query(play_list_t list); // 查询剧目界面

#endif
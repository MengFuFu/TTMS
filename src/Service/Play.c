
#include "Play.h"
#include "../Common/list.h"
#include "../Persistence/Play_Persist.h"
#include <string.h>

/*
 * Function:    Play_Srv_Add
 * Function ID:	TTMS_SCU_Play_Srv_Add
 * Description: 添加一条剧目信息
 * Input:       待添加的剧目信息数据
 * Output:      无
 * Return:      添加的记录数
 */
int Play_Srv_Add(play_t* data) {
    if (!data) return 0;

    play_list_t tempList;
    List_Init(tempList, play_node_t);
    Play_Perst_SelAll(tempList);

    int maxID = 0;
    play_node_t* pos;
    List_ForEach(tempList, pos) {
        // ✅ 防御：只取有效ID，避免读未初始化的垃圾值
        if (pos && pos->data.id > maxID && pos->data.id != 0xCCCCCCCC) {
            maxID = pos->data.id;
        }
    }
    data->id = maxID + 1; // ✅ 这里一定是正整数

    List_Destroy(tempList, play_node_t);
    return Play_Perst_Insert(data);
}

/*
 * Function:    Play_Srv_Modify
 * Function ID:	TTMS_SCU_Play_ Srv _Mod
 * Description: 更新一条剧目信息
 * Input:       待更新的剧目信息数据
 * Output:      无
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_Srv_Modify(play_t* data) {
	return Play_Perst_Update(data);
}

/*
 * Function:    Play_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Play_Srv_DelByID
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      无
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_Srv_DeleteByID(int ID) {
	return Play_Perst_DeleteByID(ID);
}

/*
 * Function:    Play_Srv_FetchAll
 * Function ID:	TTMS_SCU_Play_Srv_FetchAll
 * Description: 将所有剧目信息建立成一条链表
 * Input:       list剧目信息链表的头指针
 * Output:      无
 * Return:      返回查找到的记录数目
 */
int Play_Srv_FetchAll(play_list_t list) {
	return Play_Perst_SelAll(list);
}

int Play_Srv_FetchByID(int id, play_t* buf) {
    if (!buf) return 0;
    // ✅ 清0，避免返回垃圾数据
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
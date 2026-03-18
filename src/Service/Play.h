#ifndef PLAY_H
#define PLAY_H

#include "..\Common\common.h"  // �����Ŀͨ������/��ҳͷ�ļ�

// -------------------------- �������Ͷ��壨�ϸ�PPT�� --------------------------
// (1) ��Ŀ����ö��
typedef enum {
    PLAY_TYPE_FILM = 1,
    PLAY_TYPE_OPEAR = 2,  // ע�⣺PPT����OPEAR������OPERA��
    PLAY_TYPE_CONCERT = 3
} play_type_t;

// (2) ��Ŀ�ּ�ö��
typedef enum {
    PLAY_RATE_CHILD = 1,
    PLAY_RATE_TEENAGE = 2,
    PLAY_RATE_ADULT = 3
} play_rating_t;

// (4) ��Ŀʵ��ṹ��
typedef struct {
    int          id;          // ��ĿID
    char         name[31];    // ��Ŀ����
    play_type_t  type;        // ��Ŀ����
    char         area[9];     // ��Ŀ��Ʒ����
    play_rating_t rating;     // ��Ŀ�ȼ�
    int          duration;    // ʱ�������ӣ�
    user_date_t  start_date;  // ��ʼ��ӳ����
    user_date_t  end_date;    // ��ӳ��������
    int          price;       // ����Ʊ��
} play_t;

// (5) ��Ŀ�����ڵ�
typedef struct play_node {
    play_t          data;  // ʵ������
    struct play_node* next;// ����ָ��
    struct play_node* prev;// ǰ��ָ��
} play_node_t, * play_list_t;

// -------------------------- ҵ��㺯����������Srv�� --------------------------
int  Play_Srv_FetchAll(play_list_t list);
int  Play_Srv_Add(play_t* data);
int  Play_Srv_Modify(play_t* data);
int  Play_Srv_DeleteByID(int id);
int  Play_Srv_FetchByID(int id, play_t* buf);

#endif
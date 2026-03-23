#ifndef PLAY_H
#define PLAY_H

#include "../Common/common.h"

typedef enum {
    PLAY_TYPE_FILM = 1,
    PLAY_TYPE_OPEAR = 2,
    PLAY_TYPE_CONCERT = 3
} play_type_t;

typedef enum {
    PLAY_RATE_CHILD = 1,
    PLAY_RATE_TEENAGE = 2,
    PLAY_RATE_ADULT = 3
} play_rating_t;

typedef struct {
    int          id;
    char         name[31];
    play_type_t  type;
    char         area[9];
    play_rating_t rating;
    int          duration;
    user_date_t  start_date;
    user_date_t  end_date;
    int          price;
} play_t;

typedef struct play_node {
    play_t          data;
    struct play_node* next;
    struct play_node* prev;
} play_node_t, * play_list_t;

int Play_Srv_FetchAll(play_list_t list);
int Play_Srv_Add(play_t* data);
int Play_Srv_Modify(play_t* data);
int Play_Srv_DeleteByID(int id);
int Play_Srv_FetchByID(int id, play_t* buf);

#endif

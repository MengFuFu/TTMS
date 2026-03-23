/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: List.h
* Description: Linked list macros and pagination
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <assert.h>

#define List_Init(list, list_node_t) {                    \
        list = (list_node_t*)malloc(sizeof(list_node_t)); \
        (list)->next = (list)->prev = list;                    \
    }

#define List_Free(list, list_node_t) {            \
        assert(NULL != list);                        \
        list_node_t *tmpPtr;                    \
        (list)->prev->next = NULL;                 \
        while (NULL != (tmpPtr = (list)->next)) {    \
            (list)->next = tmpPtr->next;         \
            free(tmpPtr);                      \
        }                                       \
        (list)->next = (list)->prev = list;         \
    }

#define List_Destroy(list, list_node_t) {        \
        assert(NULL != list);                        \
        List_Free(list, list_node_t)            \
        free(list);                             \
        (list) = NULL;                            \
    }

#define List_AddHead(list, newNode) {            \
        (newNode)->next = (list)->next;         \
        (list)->next->prev = newNode;         \
        (newNode)->prev = (list);             \
        (list)->next = newNode;             \
    }

#define List_AddTail(list, newNode) {            \
        (newNode)->prev = (list)->prev;         \
        (list)->prev->next = newNode;         \
        (newNode)->next = list;             \
        (list)->prev = newNode;             \
    }

#define List_InsertBefore(node, newNode) {        \
        (newNode)->prev = (node)->prev;         \
        (newNode)->next = node;             \
        (newNode)->prev->next = newNode;         \
        (newNode)->next->prev = newNode;         \
    }

#define List_InsertAfter(node, newNode) {        \
        (newNode)->next = node->next;         \
        (newNode)->prev = node;              \
        (newNode)->next->prev = newNode;         \
        (newNode)->prev->next = newNode;         \
    }

#define List_IsEmpty(list)  ((list != NULL) \
    && ((list)->next == list)             \
    && (list == (list)->prev))

#define List_DelNode(node) {\
            assert(NULL != node && node != (node)->next && node != (node)->prev);                \
            (node)->prev->next = (node)->next;     \
            (node)->next->prev = (node)->prev;    \
    }

#define List_FreeNode(node) {    \
        List_DelNode(node);     \
        free(node);             \
    }

#define List_ForEach(list, curPos)         \
     for (   curPos = (list)->next;      \
                   curPos != list;       \
                   curPos = curPos->next   \
        )

typedef struct {
    int totalRecords;
    int offset;
    int pageSize;
    void *curPos;
} Pagination_t;

#define List_Paging(list, paging, list_node_t) {            \
        if (paging.offset + paging.pageSize >= paging.totalRecords) {   \
            Paging_Locate_LastPage(list, paging, list_node_t); }   \
        else {                                                  \
            int i;                                              \
            list_node_t * pos = (list)->next;                     \
            for (i = 0; i < paging.offset && pos != list; i++)        \
               pos = pos->next;                             \
            paging.curPos = (void*)pos;                           \
        }                                                       \
    }

#define Paging_Locate_FirstPage(list, paging) { \
        paging.offset = 0;                        \
        paging.curPos = (void *)((list)->next);    \
    }

#define Paging_Locate_LastPage(list, paging, list_node_t) {    \
    int i = paging.totalRecords % paging.pageSize;   \
    if (0 == i && paging.totalRecords > 0)             \
        i = paging.pageSize;                         \
    paging.offset = paging.totalRecords - i;        \
    list_node_t * pos = (list)->prev;                 \
    for (; i > 1; i--)                                   \
        pos = pos->prev;                             \
    paging.curPos = (void*)pos;                           \
                                                    \
}

#define Paging_ViewPage_ForEach(list, paging, list_node_t, pos, i)  \
    for (i = 0, pos = (list_node_t *) (paging.curPos);    \
            pos != list && i < paging.pageSize;         \
            i++, pos = pos->next)                         \

#define Paging_Locate_OffsetPage(list, paging, offsetPage, list_node_t) {\
    int offset = offsetPage * paging.pageSize;          \
    list_node_t *pos = (list_node_t *)paging.curPos;   \
    int i;                                             \
    if (offset > 0) {                                  \
        if (paging.offset + offset >= paging.totalRecords) {   \
            Paging_Locate_LastPage(list, paging, list_node_t);   \
        } else {                                             \
            for (i = 0; i < offset; i++)                      \
                pos = pos->next;                             \
            paging.offset += offset;                       \
            paging.curPos = (void *)pos;                     \
        }                                                   \
    } else {                                                  \
        if (paging.offset + offset <= 0) {                 \
            Paging_Locate_FirstPage(list, paging);         \
        } else {                                             \
            for (i = offset; i < 0; i++)                      \
                pos = pos->prev;                            \
            paging.offset += offset;                       \
            paging.curPos = pos;                             \
        }                                                   \
    }                                                       \
}

#define Pageing_CurPage(paging)     (0 == (paging).totalRecords ? 0 : 1 + (paging).offset / (paging).pageSize)
#define Pageing_TotalPages(paging) (((paging).totalRecords % (paging).pageSize == 0) ? \
    (paging).totalRecords / (paging).pageSize : \
    (paging).totalRecords / (paging).pageSize + 1)

#define Pageing_IsFirstPage(paging) (Pageing_CurPage(paging) <= 1)
#define Pageing_IsLastPage(paging)  (Pageing_CurPage(paging) >= Pageing_TotalPages(paging))

#endif

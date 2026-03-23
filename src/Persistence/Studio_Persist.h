/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: Studio_Persist.h
* Description: Studio persistence layer header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef STUDIO_PERSIST_H_
#define STUDIO_PERSIST_H_

#include "../Service/Studio.h"

int Studio_Perst_Insert(studio_t *data);
int Studio_Perst_Update(const studio_t *data);
int Studio_Perst_DeleteByID(int ID);
int Studio_Perst_SelectByID(int ID, studio_t *buf);
int Studio_Perst_SelectAll(studio_list_t list);

#endif

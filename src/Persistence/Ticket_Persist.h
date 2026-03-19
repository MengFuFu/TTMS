#pragma once
#include "../Service/Ticket.h"



//功能：向文件中存储新票 
int Ticket_Perst_Insert(ticket_t* data);

//功能：在文件中更新票
int Ticket_Perst_Update(const ticket_t* data);

//功能：在文件中删除票
int Ticket_Perst_DeleteByID(int ID);

//从文件中载入一条票的数据
int Studio_Perst_SelectByID(int ID, ticket_t* buf);

//功能：从文件中载入全部票
int Ticket_Perst_SelectAll(ticket_list_t list);



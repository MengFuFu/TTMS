#ifndef SALESANALYSIS_H_
#define SALESANALYSIS_H_
#include "../Common/common.h"
//#include "../Service/Play.h"
//#include "../Service/Sale.h"
typedef struct {
	int plar_id;
	char name[31];
	char area[9];
	int duration;
	long totaltickets;
	long sales;
	int price;
	user_date_t start_date;
	user_date_t end_date;
}salesanalysis_t;
typedef struct salesanalysis_tnode {
	salesanalysis_t date;
	struct salesanalysis_node* next, * prev;
}salesanalysis_node_t, * salesanalysis_list_t;
int Salesanalysis_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate);
int StaSales_Srv_CompSaleVal(int userID, user_date_t stDate, user_date_t endDate);
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list);
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);
#endif
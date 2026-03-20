#ifndef SALESANALYSIS_UI_H_
#define SALESANALYSIS_UI_H_
#include "../Common/list.h"
//#include "../Service/account.h"
//#include "../Service/play.h"
//#include "../Service/Sale.h"
//#include "../Service/saleanalysis.h"
void SalesAnalysis_UI_BoxOffice();
void SalesAnalysis_UI_StaSale(int userID, user_date_t stDate, user_date_t endDate);
void SalesAnalysis_UI_MgtEn();

static const int STASALES_PAGE_SIZE = 6;
void StaSales_UI_MgtEnt();
void StaSales_UI_Self();
void StaSales_UI_Clerk();
#endif

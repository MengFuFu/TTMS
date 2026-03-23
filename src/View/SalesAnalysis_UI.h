#ifndef SALESANALYSIS_UI_H_
#define SALESANALYSIS_UI_H_
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Service/Account.h"

void SalesAnalysis_UI_BoxOffice(void);
void SalesAnalysis_UI_StaSale(int userID, user_date_t stDate, user_date_t endDate);
void SalesAnalysis_UI_MgtEn(void);
void StaSales_UI_MgtEnt(account_t CurUser);
void StaSales_UI_Self(void);
void StaSales_UI_Clerk(void);

#endif

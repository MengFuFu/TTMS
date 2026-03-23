/*
* Copyright(C), 2007-2008, XUPT Univ.
* File name: SalesAnalysis_Perst.h
* Description: Sales analysis persistence header file
* Author:   XUPT
* Version:  v.1
* Date:     2015/04/22
*/

#ifndef SALESANALYSISPERSIST_H_
#define SALESANALYSISPERSIST_H_

#include "../Service/SalesAnalysis.h"
#include "../Service/Sale.h"

// Identifier: TTMS_SCU_SalesAnalysis_Perst_Insert
// Function: Insert a new sales analysis record
int SalesAnalysis_Perst_Insert(const salesanalysis_t* data);

// Identifier: TTMS_SCU_SalesAnalysis_Perst_SelectAll
// Function: Select all sales analysis records into a list
int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list);

#endif

#define _CRT_SECURE_NO_WARNINGS
#include "../Persistence/EntityKey_Persist.h"
#include "../Service/SalesAnalysis.h"
#include "../Service/Play.h"
#include "../Service/Sale.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static const char SALESANALYSIS_DATA_FILE[] = "salesanalysis.dat";

int SalesAnalysis_Perst_Insert(const salesanalysis_t* data) {
    assert(NULL != data);
    FILE* fp = fopen(SALESANALYSIS_DATA_FILE, "ab");
    int rtn = 0;
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SALESANALYSIS_DATA_FILE);
        return 0;
    }
    rtn = fwrite(data, sizeof(salesanalysis_t), 1, fp);
    fclose(fp);
    return rtn;
}

int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list) {
    salesanalysis_node_t* newNode;
    salesanalysis_t data;
    int recCount = 0;
    assert(NULL != list);
    List_Free(list, salesanalysis_node_t);
    FILE* fp = fopen(SALESANALYSIS_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }
    while (!feof(fp)) {
        if (fread(&data, sizeof(salesanalysis_t), 1, fp)) {
            newNode = (salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));
            if (!newNode) {
                printf("Warning!\n");
                break;
            }
            newNode->data = data;
            newNode->next = newNode->prev = newNode;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}

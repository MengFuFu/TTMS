#define _CRT_SECURE_NO_WARNINGS
#include "../View/Main_Menu.h"
#include <stdio.h>
#include "../View/Studio_UI.h"
#include "../View/Play_UI.h"
#include "../View/Sale_UI.h"
#include "../View/Account_UI.h"
#include "../View/Schedule_UI.h"
#include "../View/Ticket_UI.h"
#include "../View/SalesAnalysis_UI.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"

void Main_Menu(void) {
    char choice[10];
    do {
        printf("\n==================================================================\n");
        printf("**************** Theater Ticket Management System ****************\n");
        printf("[S]tudio Management.\n");
        printf("[P]lay Management.\n");
        printf("[C]reate Schedule.\n");
        printf("[T]icket Sale.\n");
        printf("[R]eturn Ticket.\n");
        printf("[Q]ueries\n");
        printf("Ra[n]king and Statistics.\n");
        printf("[A]ccount Management.\n");
        printf("[E]xit.\n");
        printf("\n==================================================================\n");
        readString(choice, sizeof(choice), "Please input your choice : ");
        switch (choice[0]) {
        case 'S':
        case 's':
            Studio_UI_MgtEntry();
            break;
        case 'P':
        case 'p':
            Play_UI_MgtEntry();
            break;
        case 'C':
        case 'c':
            {
                // 先显示剧目列表，让用户选择剧目
                play_list_t playList;
                List_Init(playList, play_node_t);
                int playCount = Play_Srv_FetchAll(playList);
                
                if (playCount <= 0) {
                    printf("No plays available! Please add plays first.\n");
                    char temp[10];
                    readString(temp, sizeof(temp), "Press Enter to return...");
                    List_Destroy(playList, play_node_t);
                    break;
                }
                
                printf("\n==========================================================================================================\n");
                printf("****************************************  Play List  ****************************************\n");
                printf("%5s  %-20s\n", "ID", "Name");
                printf("----------------------------------------------------------------------------------------------------------\n");
                
                play_node_t* pos;
                List_ForEach(playList, pos) {
                    printf("%5d  %-20s\n", pos->data.id, pos->data.name);
                }
                
                printf("----------------------------------------------------------------------------------------------------------\n");
                printf("Total plays: %d\n", playCount);
                printf("==========================================================================================================\n");
                
                int play_id = readInt("Input Play ID to manage schedules: ");
                Schedule_UI_MgtEntry(play_id);
                List_Destroy(playList, play_node_t);
            }
            break;
        case 'T':
        case 't':
            Sale_UI_MgtEntry();
            break;
        case 'R':
        case 'r':
            Sale_UI_ReturnTicket();
            break;
        case 'Q':
        case 'q':
            printf("Query function - select play first.\n");
            break;
        case 'N':
        case 'n':
            SalesAnalysis_UI_MgtEn();
            break;
        case 'A':
        case 'a':
            Account_UI_MgtEntry();
            break;
        }
    } while ('E' != choice[0] && 'e' != choice[0]);
}

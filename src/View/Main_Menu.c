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
#include "../View/Query_UI.h"
#include "../Service/Play.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include "../SafeStdin/safe_stdin.h"

void Main_Menu(void) {
    char choice[10];
    do {
        system("cls");
        printf("\n==================================================================\n");
        printf("**************** Theater Ticket Management System ****************\n");
        printf("Welcome, %s! (Role: ", gl_CurUser.username);
        
        // 根据用户角色显示不同的菜单
        switch (gl_CurUser.type) {
        case USR_ADMIN:
            printf("Admin)\n");
            printf("[S]tudio Management.\n");
            printf("[P]lay Management.\n");
            printf("[C]reate Schedule.\n");
            printf("[T]icket Sale.\n");
            printf("[Q]ueries\n");
            printf("Ra[n]king and Statistics.\n");
            printf("[A]ccount Management.\n");
            printf("[E]xit.\n");
            printf("[L]ogout.\n");
            break;
        case USR_MANG:
            printf("Manager)\n");
            printf("[S]tudio Query.\n");
            printf("[P]lay Management.\n");
            printf("[C]reate Schedule.\n");
            printf("[T]icket Sale.\n");
            printf("[Q]ueries\n");
            printf("Ra[n]king and Statistics.\n");
            printf("[A]ccount Management (Clerk).\n");
            printf("[E]xit.\n");
            printf("[L]ogout.\n");
            break;
        case USR_CLERK:
            printf("Clerk)\n");
            printf("[S]tudio Query.\n");
            printf("[P]lay Query.\n");
            printf("[T]icket Sale.\n");
            printf("[Q]ueries\n");
            printf("[K]Ticket Information.\n");
            printf("[Z]Seat Information.\n");
            printf("[E]xit.\n");
            printf("[L]ogout.\n");
            break;
            default:
                printf("Unknown)\n");
                printf("[E]xit.\n");
                break;
        }
        
        printf("\n==================================================================\n");
        readString(choice, sizeof(choice), "Please input your choice : ");
        
        switch (choice[0]) {
        case 'S':
        case 's':
            // 所有角色都可以查看影厅信息
            Studio_UI_MgtEntry();
            break;
        case 'P':
        case 'p':
            if (gl_CurUser.type == USR_ADMIN || gl_CurUser.type == USR_MANG) {
                Play_UI_MgtEntry();
            } else {
                // 售票员只能查询
                printf("You can only query play information.\n");
                printf("Press any key to return...");
                _getch();
            }
            break;
        case 'C':
        case 'c':
            if (gl_CurUser.type == USR_ADMIN || gl_CurUser.type == USR_MANG) {
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
            } else {
                printf("You don't have permission to create schedules.\n");
                printf("Press any key to return...");
                _getch();
            }
            break;
        case 'T':
        case 't':
            Sale_UI_MgtEntry();
            break;
        case 'Q':
        case 'q':
            Query_UI_MgtEntry();
            break;
        case 'N':
        case 'n':
            if (gl_CurUser.type == USR_ADMIN || gl_CurUser.type == USR_MANG) {
                SalesAnalysis_UI_MgtEn();
            } else {
                printf("You don't have permission to view sales statistics.\n");
                printf("Press any key to return...");
                _getch();
            }
            break;
        case 'A':
        case 'a':
            if (gl_CurUser.type == USR_ADMIN) {
                Account_UI_MgtEntry();
            } else if (gl_CurUser.type == USR_MANG) {
                printf("You can only manage clerk accounts.\n");
                Account_UI_MgtEntry();
            } else {
                printf("You don't have permission to manage accounts.\n");
                printf("Press any key to return...");
                _getch();
            }
            break;
        case 'K':
        case 'k':
            if (gl_CurUser.type == USR_CLERK) {
                // 售票员查询票信息
                Ticket_UI_ListTickets();
            } else {
                printf("Invalid choice!\n");
                printf("Press any key to continue...");
                _getch();
            }
            break;
        case 'Z':
        case 'z':
            if (gl_CurUser.type == USR_CLERK) {
                // 售票员查询座位信息
                printf("\n================ Seat Information =================\n");
                // 这里可以添加座位查询逻辑
                printf("Seat information query function to be implemented.\n");
                printf("Press any key to return...");
                _getch();
            } else {
                printf("Invalid choice!\n");
                printf("Press any key to continue...");
                _getch();
            }
            break;
        case 'L':
        case 'l':
            // 清除登录状态
            Account_Srv_ClearLoginStatus();
            printf("You have been logged out.\n");
            printf("Press any key to return to login screen...");
            _getch();
            // 重新调用登录函数
            if (!SysLogin()) {
                // 登录失败，退出程序
                choice[0] = 'e'; // 设置为退出
            }
            break;
        }
    } while ('E' != choice[0] && 'e' != choice[0]);
}

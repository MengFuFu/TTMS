#define _CRT_SECURE_NO_WARNINGS

#include"Account_UI.h"
#include "../Service/account.h"
#include"../Common/List.h"
#include"../Service/Account.h"
#include"../Persistence/EntityKey_Persist.h"
#include<windows.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<time.h>
#include<conio.h>  

// 全局用户变量声明
extern account_t gl_CurUser;

// 【修正1】const常量必须初始化，每页显示5条
static const int ACCOUNT_PAGE_SIZE;

// 安全清空输入缓冲区的通用函数（避免无内容时阻塞）
static void Safe_Flush_Stdin()
{
	// 只有缓冲区有内容时才清空，避免阻塞
	while (_kbhit())
	{
		_getch();
	}
}

// 登录核心函数
int SysLogin()
{
	char ch;
	int count = 0;
	char username[20] = { 0 };
	char password[20] = { 0 };
	account_t buf;

	printf("\t\t\t=====================================================================\n");
	printf("\n");
	// 【修正2】提示语末尾不加\n，光标停在冒号后
	printf("\t\t\t 请输入账号：");
	scanf("%19s", username);
	// 输入后清空缓冲区残留的换行符
	while (getchar() != '\n');

	// 【修正2】提示语末尾不加\n，光标停在冒号后
	printf("\t\t\t 请输入密码：");
	// 密码隐藏输入逻辑
	while (1)
	{
		ch = _getch();
		// 回车结束输入
		if (ch == '\r' || ch == '\n')
		{
			password[count] = '\0';
			printf("\n");
			break;
		}
		// 退格处理
		if (ch == '\b')
		{
			if (count > 0)
			{
				printf("\b \b");
				count--;
			}
		}
		// 正常字符输入
		else
		{
			if (count < 19)
			{
				password[count++] = ch;
				printf("*");
			}
		}
	}
	printf("\t\t\t=====================================================================\n");

	if (Account_Srv_Verify(username, password))
	{
		printf("登陆成功！(´∩｡• ᵕ •｡∩`)\n");
		Account_Srv_FetchByName(username, &buf);
		strcpy(gl_CurUser.username, buf.username);
		strcpy(gl_CurUser.password, buf.password);
		gl_CurUser.id = buf.id;
		gl_CurUser.type = buf.type;
		system("pause");
		return 1;
	}
	printf("账号或密码错误！\n");
	system("pause");
	return 0;
}

// 账号管理入口
void Account_UI_MgtEntry()
{
	int i;
	char choice = '0';
	char username[30] = { 0 };
	account_list_t head;
	account_node_t* pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do
	{
		system("cls");

		printf("\n=================================================\n");
		printf(" 用户信息\n");
		printf("%5s %18s %10s %10s \n", "ID", "用户类型", "姓名", "密码");
		printf("\n-------------------------------------------------\n");
		// 分页遍历逻辑
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i)
		{
			printf("%5d %18d %10s %10s \n", pos->data.id, pos->data.type, pos->data.username, pos->data.password);
		}

		printf("-------------------第%2d/%2d页---------------------\n",
			Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("***************************************************\n");
		printf("[P]上一页 [N]下一页 [A]添加 [D]删除 [U]更新 [S]查询 [R]返回上一页\n");
		printf("\n=====================================================\n");
		// 【修正2】提示语末尾不加\n，光标停在冒号后
		printf("\t\t\t输入功能前的字母，选择功能：");

		// 【修正3】核心：先读取输入，再清空缓冲区，顺序完全颠倒回来
		// 前面的空格自动吸收所有残留换行/空格，不需要额外先清空
		scanf(" %c", &choice);
		// 读取后清空缓冲区残留的多余字符（比如输入AB回车，只取A，剩下的B\n清空）
		while (getchar() != '\n');

		switch (choice)
		{
		case 'a':
		case 'A':
			if (Account_UI_Add(head))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t输入要删除的用户名：");
			scanf("%29s", username);
			while (getchar() != '\n');
			if (Account_UI_Delete(head, username))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t输入要修改的用户名：");
			scanf("%29s", username);
			while (getchar() != '\n');
			if (Account_UI_Modify(head, username))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\t\t\t请输入用户姓名：");
			scanf("%29s", username);
			while (getchar() != '\n');
			if (Account_UI_Query(head, username))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		case 'r':
		case 'R':
			break;
		default:
			printf("输入错误，请重新选择！o(￣ヘ￣o#)\n");
			break;
		}

		// 【修正4】优化按键等待逻辑，不需要按两次
		printf("按任意键继续...");
		Safe_Flush_Stdin(); // 先清空残留内容
		_getch(); // 等待用户按任意键，无回显
	} while (choice != 'r' && choice != 'R');

	List_Destroy(head, account_node_t);
}

// 添加账号
int Account_UI_Add(account_list_t list)
{
	account_list_t pNew;
	char ch;
	int count = 0;
	char password[20] = { 0 };
	int type = 0;
	pNew = (account_list_t)malloc(sizeof(account_node_t));
	if (pNew == NULL)
	{
		printf("内存分配失败！\n");
		return 0;
	}
	memset(pNew, 0, sizeof(account_node_t));

	// 【修正2】核心：去掉末尾的\n，光标停在冒号后，不会换行
	printf("\t\t\t请输入新的账号：");
	scanf("%19s", pNew->data.username);
	while (getchar() != '\n');

	// 检查用户名是否已存在
	if (Account_Srv_FindByUserName(list, pNew->data.username) != NULL)
	{
		printf("\t\t\t该账号已经被注册!注册失败!o(￣ヘ￣o#)\n");
		free(pNew);
		return 0;
	}

	// 【修正2】去掉末尾的\n，光标停在冒号后
	printf("\t\t\t请输入新的密码：");
	// 密码隐藏输入逻辑
	while (1)
	{
		ch = _getch();
		if (ch == '\r' || ch == '\n')
		{
			password[count] = '\0';
			printf("\n");
			break;
		}
		if (ch == '\b')
		{
			if (count > 0)
			{
				printf("\b \b");
				count--;
			}
		}
		else
		{
			if (count < 19)
			{
				password[count++] = ch;
				printf("*");
			}
		}
	}
	strcpy(pNew->data.password, password);

	printf("请输入新账号的类型：(1.售票员 2.剧院经理 9.系统管理员)：");
	scanf("%d", &type);
	while (getchar() != '\n');

	// 用户类型赋值
	if (type == 1)
	{
		pNew->data.type = USR_CLERK;
	}
	else if (type == 2)
	{
		pNew->data.type = USR_MANG;
	}
	else if (type == 9)
	{
		pNew->data.type = USR_ADMIN;
	}
	else if (type == 0)
	{
		pNew->data.type = USR_ANOMY;
	}
	else
	{
		printf("类型输入错误，自动退出此功能\n");
		free(pNew);
		return 0;
	}

	// 主键生成
	pNew->data.id = EntKey_Perst_GetNewKeys("Account", 1);
	Account_Srv_Add(&pNew->data);
	List_AddTail(list, pNew);
	printf("注册成功\n");
	return 1;
}

// 修改账号
int Account_UI_Modify(account_list_t list, char userName[])
{
	account_list_t p = Account_Srv_FindByUserName(list, userName);
	if (p == NULL)
	{
		printf("\t\t\t该账号不存在!(。﹏。*)\n");
		return 0;
	}

	printf("当前信息：%s %s\n", p->data.username, p->data.password);
	char pwd[20] = { 0 };
	printf("\t\t\t请输入修改后的用户密码：");
	scanf("%19s", pwd);
	while (getchar() != '\n');
	strcpy(p->data.password, pwd);

	Account_Srv_Modify(&p->data);
	printf("\t\t\t修改成功(´∩｡• ᵕ •｡∩`)\n");
	return 1;
}

// 删除账号
int Account_UI_Delete(account_list_t list, char userName[])
{
	account_list_t p = Account_Srv_FindByUserName(list, userName);
	if (p == NULL)
	{
		printf("\t\t\t该账号不存在！━(￣ー￣*|||━━\n");
		return 0;
	}

	if (Account_Srv_DeleteByID(p->data.id))
	{
		List_DelNode(p);
		free(p);
		printf("删除成功\n");
		return 1;
	}
	printf("删除失败\n");
	return 0;
}

// 查询账号
int Account_UI_Query(account_list_t list, char userName[])
{
	account_list_t pTemp = Account_Srv_FindByUserName(list, userName);
	if (pTemp == NULL)
	{
		printf("该账号不存在\n");
		return 0;
	}

	printf("\n++++++++++++++++++++++++++++++++++++++\n");
	printf("     用户信息\n");
	printf("%5s %18s %10s %10s \n", "ID", "类型", "姓名", "密码");
	printf("----------------------------------------\n");
	printf("%5d %18d %10s %10s \n",
		pTemp->data.id, pTemp->data.type, pTemp->data.username, pTemp->data.password);
	return 1;
}
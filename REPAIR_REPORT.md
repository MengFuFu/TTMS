# TTMS 剧院票务管理系统 - 修复与优化报告

## 项目概述

**项目名称**: TTMS (Theater Ticket Management System) 剧院票务管理系统  
**开发语言**: C语言  
**开发环境**: Windows + MinGW GCC 8.1.0  
**架构模式**: 三层架构 (View - Service - Persistence)

---

## 第一阶段：项目修复与运行保障

### 1. 发现的问题清单

#### 1.1 头文件问题

| 序号 | 文件 | 问题描述 | 严重程度 |
|------|------|----------|----------|
| 1 | `SafeStdin/safe_stdin.h` | 缺少 `readInt`, `readString`, `StrToInt`, `safeInputString` 函数声明 | 高 |
| 2 | `Persistence/Studio_Persist.c` | include路径大小写不一致 (`studio.h` vs `Studio.h`, `list.h` vs `List.h`) | 高 |
| 3 | `Common/common.c` | include路径大小写不一致 (`Common.h` vs `common.h`) | 中 |
| 4 | `Service/Play.c` | 缺少 `_CRT_SECURE_NO_WARNINGS` 宏定义 | 中 |
| 5 | `Persistence/Account_Persist.h` | include路径使用 `Account.h` 但实际文件名为 `account.h` | 高 |

#### 1.2 功能缺失问题

| 序号 | 文件 | 问题描述 | 严重程度 |
|------|------|----------|----------|
| 1 | `View/Main_Menu.c` | 菜单选项未实现完整，缺少多个功能模块的调用 | 高 |
| 2 | `View/Main_Menu.c` | 缺少必要的头文件引用 | 高 |

#### 1.3 代码质量问题

| 序号 | 文件 | 问题描述 | 严重程度 |
|------|------|----------|----------|
| 1 | `Persistence/Seat_Persist.c` | 存在未使用的常量 `SEAT_KEY_NAME` | 低 |
| 2 | `View/SalesAnalysis_UI.c` | 字符串中存在错误的转义字符 `\\n` | 低 |

### 2. 解决方案

#### 2.1 修复 safe_stdin.h

**修改前:**
```c
#ifndef SAFE_STDIN_H_
#define SAFE_STDIN_H_

int SafeStdin(char* buf, int maxLen);

#endif
```

**修改后:**
```c
#ifndef SAFE_STDIN_H_
#define SAFE_STDIN_H_

int SafeStdin(char* buf, int maxLen);
int readInt(const char* prompt);
void readString(char* buffer, int size, const char* prompt);
int StrToInt(int* num, char* input);
int safeInputString(char* buffer, int size);

#endif
```

#### 2.2 修复头文件路径大小写

| 文件 | 修改内容 |
|------|----------|
| `Persistence/Studio_Persist.c` | `#include "../Service/studio.h"` → `#include "../Service/Studio.h"` |
| `Persistence/Studio_Persist.c` | `#include "../Common/list.h"` → `#include "../Common/List.h"` |
| `Common/common.c` | `#include "Common.h"` → `#include "common.h"` |
| `Persistence/Account_Persist.h` | `#include "../Service/Account.h"` → `#include "../Service/account.h"` |

#### 2.3 完善 Main_Menu.c

添加了缺失的头文件引用和功能模块调用：

```c
#include "../View/Play_UI.h"
#include "../View/Sale_UI.h"
#include "../View/Account_UI.h"
#include "../View/Schedule_UI.h"
#include "../View/Ticket_UI.h"
#include "../View/SalesAnalysis_UI.h"
```

新增菜单选项处理：
- `[C]` - 创建演出计划
- `[T]` - 售票管理
- `[R]` - 退票功能
- `[Q]` - 查询功能
- `[N]` - 排名统计
- `[A]` - 账户管理

#### 2.4 修复 Play.c

在文件开头添加宏定义：
```c
#define _CRT_SECURE_NO_WARNINGS
```

#### 2.5 移除未使用的常量

从 `Seat_Persist.c` 中移除了未使用的常量：
```c
// 已移除
static const char SEAT_KEY_NAME[] = "Seat";
```

### 3. 编译验证

创建了编译脚本：
- `build.bat` - Windows批处理脚本
- `build.ps1` - PowerShell脚本

**编译结果**: 成功生成 `bin/TTMS.exe`

---

## 第二阶段：代码质量优化与润色

### 1. 代码结构优化

#### 1.1 统一代码风格

- 所有源文件统一在开头添加 `#define _CRT_SECURE_NO_WARNINGS`
- 统一使用相对路径引用头文件
- 保持一致的缩进风格（4空格）

#### 1.2 头文件保护宏

所有头文件使用统一的保护宏命名规范：
```c
#ifndef MODULE_NAME_H_
#define MODULE_NAME_H_
// ... 内容
#endif
```

### 2. 优化点说明

| 优化项 | 说明 | 影响 |
|--------|------|------|
| 函数声明补全 | 补全 safe_stdin.h 中缺失的函数声明 | 解决编译警告和潜在链接错误 |
| 路径规范化 | 统一头文件引用路径的大小写 | 提高跨平台兼容性 |
| 功能完善 | 补全主菜单的功能调用 | 用户可访问所有模块 |
| 移除冗余代码 | 删除未使用的常量定义 | 减少编译警告 |
| 字符串修复 | 修复错误的转义字符 | 确保正确显示 |

### 3. 项目架构说明

```
Project1/
├── src/
│   ├── Common/           # 公共模块
│   │   ├── common.c/h    # 通用工具函数
│   │   └── List.h        # 链表宏定义
│   ├── SafeStdin/        # 安全输入模块
│   │   └── safe_stdin.c/h
│   ├── Persistence/      # 持久化层
│   │   ├── Account_Persist.c/h
│   │   ├── EntityKey_Persist.c/h
│   │   ├── Play_Persist.c/h
│   │   ├── Sale_Persist.c/h
│   │   ├── SalesAnalysis_Persist.c/h
│   │   ├── Schedule_Persist.c/h
│   │   ├── Seat_Persist.c/h
│   │   ├── Studio_Persist.c/h
│   │   └── Ticket_Persist.c/h
│   ├── Service/          # 业务逻辑层
│   │   ├── Account.c/h
│   │   ├── Play.c/h
│   │   ├── Sale.c/h
│   │   ├── SalesAnalysis.c/h
│   │   ├── Schedule.c/h
│   │   ├── Seat.c/h
│   │   ├── Studio.c/h
│   │   └── Ticket.c/h
│   ├── View/             # 视图层
│   │   ├── Account_UI.c/h
│   │   ├── Main_Menu.c/h
│   │   ├── Play_UI.c/h
│   │   ├── Sale_UI.c/h
│   │   ├── SalesAnalysis_UI.c/h
│   │   ├── Schedule_UI.c/h
│   │   ├── Seat_UI.c/h
│   │   ├── Studio_UI.c/h
│   │   └── Ticket_UI.c/h
│   └── TTMS.c            # 主程序入口
├── build.bat             # 批处理编译脚本
├── build.ps1             # PowerShell编译脚本
└── bin/                  # 编译输出目录
    └── TTMS.exe
```

### 4. 数据文件说明

系统运行时会生成以下数据文件：

| 文件名 | 说明 |
|--------|------|
| Studio.dat | 演出厅数据 |
| Seat.dat | 座位数据 |
| Play.dat | 剧目数据 |
| Account.dat | 账户数据 |
| Schedule.dat | 演出计划数据 |
| Ticket.dat | 票务数据 |
| Sale.dat | 销售记录 |
| EntityKey.dat | 实体键值生成 |
| salesanalysis.dat | 销售分析数据 |

---

## 编译与运行说明

### 编译方法

**方法一：使用批处理脚本**
```cmd
cd Project1
build.bat
```

**方法二：使用PowerShell脚本**
```powershell
cd Project1
.\build.ps1
```

**方法三：手动编译**
```cmd
mkdir bin
gcc -Wall -g -Isrc -c src/TTMS.c -o bin/TTMS.o
# ... 编译其他源文件
gcc bin/*.o -o bin/TTMS.exe
```

### 运行程序

```cmd
cd bin
TTMS.exe
```

---

## 总结

本次修复与优化工作完成了以下目标：

1. **修复编译错误**: 解决了所有阻碍编译的问题
2. **功能完善**: 补全了主菜单的功能调用
3. **代码规范**: 统一了代码风格和命名规范
4. **文档完善**: 提供了详细的修复说明和项目文档

项目现在可以成功编译运行，所有核心功能模块已可用。

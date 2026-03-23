@echo off
echo Compiling TTMS (Theater Ticket Management System)...
echo.

set SRC_DIR=src
set OUT_DIR=bin
set CC=gcc
set CFLAGS=-Wall -g -D_CRT_SECURE_NO_WARNINGS
set LDFLAGS=

if not exist %OUT_DIR% mkdir %OUT_DIR%

echo Compiling Common modules...
%CC% %CFLAGS% -c %SRC_DIR%/Common/common.c -o %OUT_DIR%/common.o
%CC% %CFLAGS% -c %SRC_DIR%/Common/List.h -o %OUT_DIR%/List_dummy.o 2>nul

echo Compiling SafeStdin module...
%CC% %CFLAGS% -c %SRC_DIR%/SafeStdin/safe_stdin.c -o %OUT_DIR%/safe_stdin.o

echo Compiling Persistence modules...
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/EntityKey_Persist.c -o %OUT_DIR%/EntityKey_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Studio_Persist.c -o %OUT_DIR%/Studio_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Seat_Persist.c -o %OUT_DIR%/Seat_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Play_Persist.c -o %OUT_DIR%/Play_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Account_Persist.c -o %OUT_DIR%/Account_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Schedule_Persist.c -o %OUT_DIR%/Schedule_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Ticket_Persist.c -o %OUT_DIR%/Ticket_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/Sale_Persist.c -o %OUT_DIR%/Sale_Persist.o
%CC% %CFLAGS% -c %SRC_DIR%/Persistence/SalesAnalysis_Persist.c -o %OUT_DIR%/SalesAnalysis_Persist.o

echo Compiling Service modules...
%CC% %CFLAGS% -c %SRC_DIR%/Service/Studio.c -o %OUT_DIR%/Studio.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Seat.c -o %OUT_DIR%/Seat.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Play.c -o %OUT_DIR%/Play.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Account.c -o %OUT_DIR%/Account.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Schedule.c -o %OUT_DIR%/Schedule.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Ticket.c -o %OUT_DIR%/Ticket.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/Sale.c -o %OUT_DIR%/Sale.o
%CC% %CFLAGS% -c %SRC_DIR%/Service/SalesAnalysis.c -o %OUT_DIR%/SalesAnalysis.o

echo Compiling View modules...
%CC% %CFLAGS% -c %SRC_DIR%/View/Main_Menu.c -o %OUT_DIR%/Main_Menu.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Studio_UI.c -o %OUT_DIR%/Studio_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Seat_UI.c -o %OUT_DIR%/Seat_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Play_UI.c -o %OUT_DIR%/Play_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Account_UI.c -o %OUT_DIR%/Account_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Schedule_UI.c -o %OUT_DIR%/Schedule_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Ticket_UI.c -o %OUT_DIR%/Ticket_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/Sale_UI.c -o %OUT_DIR%/Sale_UI.o
%CC% %CFLAGS% -c %SRC_DIR%/View/SalesAnalysis_UI.c -o %OUT_DIR%/SalesAnalysis_UI.o

echo Compiling main program...
%CC% %CFLAGS% -c %SRC_DIR%/TTMS.c -o %OUT_DIR%/TTMS.o

echo Linking...
%CC% %OUT_DIR%/*.o -o %OUT_DIR%/TTMS.exe %LDFLAGS%

if exist %OUT_DIR%/TTMS.exe (
    echo.
    echo ========================================
    echo Compilation successful!
    echo Output: %OUT_DIR%/TTMS.exe
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Compilation failed!
    echo ========================================
)

echo.
pause

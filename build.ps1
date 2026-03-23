# TTMS Build Script for PowerShell
# Theater Ticket Management System

$ErrorActionPreference = "Continue"

$SRC_DIR = "src"
$OUT_DIR = "bin"
$CC = "gcc"
$CFLAGS = "-Wall", "-g", "-Isrc"

Write-Host "Compiling TTMS (Theater Ticket Management System)..." -ForegroundColor Green
Write-Host ""

# Create output directory if not exists
if (-not (Test-Path $OUT_DIR)) {
    New-Item -ItemType Directory -Force -Path $OUT_DIR | Out-Null
}

$objectFiles = @()

# Compile Common modules
Write-Host "Compiling Common modules..." -ForegroundColor Yellow
& $CC $CFLAGS -c "$SRC_DIR/Common/common.c" -o "$OUT_DIR/common.o"
$objectFiles += "$OUT_DIR/common.o"

# Compile SafeStdin module
Write-Host "Compiling SafeStdin module..." -ForegroundColor Yellow
& $CC $CFLAGS -c "$SRC_DIR/SafeStdin/safe_stdin.c" -o "$OUT_DIR/safe_stdin.o"
$objectFiles += "$OUT_DIR/safe_stdin.o"

# Compile Persistence modules
Write-Host "Compiling Persistence modules..." -ForegroundColor Yellow
$persistenceFiles = @(
    "EntityKey_Persist", "Studio_Persist", "Seat_Persist", "Play_Persist",
    "Account_Persist", "Schedule_Persist", "Ticket_Persist", "Sale_Persist",
    "SalesAnalysis_Persist"
)
foreach ($file in $persistenceFiles) {
    & $CC $CFLAGS -c "$SRC_DIR/Persistence/$file.c" -o "$OUT_DIR/$file.o"
    $objectFiles += "$OUT_DIR/$file.o"
}

# Compile Service modules
Write-Host "Compiling Service modules..." -ForegroundColor Yellow
$serviceFiles = @(
    "Studio", "Seat", "Play", "Account", "Schedule", "Ticket", "Sale", "SalesAnalysis"
)
foreach ($file in $serviceFiles) {
    & $CC $CFLAGS -c "$SRC_DIR/Service/$file.c" -o "$OUT_DIR/$file.o"
    $objectFiles += "$OUT_DIR/$file.o"
}

# Compile View modules
Write-Host "Compiling View modules..." -ForegroundColor Yellow
$viewFiles = @(
    "Main_Menu", "Studio_UI", "Seat_UI", "Play_UI", "Account_UI",
    "Schedule_UI", "Ticket_UI", "Sale_UI", "SalesAnalysis_UI"
)
foreach ($file in $viewFiles) {
    & $CC $CFLAGS -c "$SRC_DIR/View/$file.c" -o "$OUT_DIR/$file.o"
    $objectFiles += "$OUT_DIR/$file.o"
}

# Compile main program
Write-Host "Compiling main program..." -ForegroundColor Yellow
& $CC $CFLAGS -c "$SRC_DIR/TTMS.c" -o "$OUT_DIR/TTMS.o"
$objectFiles += "$OUT_DIR/TTMS.o"

# Linking
Write-Host "Linking..." -ForegroundColor Yellow
& $CC $objectFiles -o "$OUT_DIR/TTMS.exe"

# Check result
if (Test-Path "$OUT_DIR/TTMS.exe") {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "Compilation successful!" -ForegroundColor Green
    Write-Host "Output: $OUT_DIR/TTMS.exe" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "Compilation failed!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
}

Write-Host ""

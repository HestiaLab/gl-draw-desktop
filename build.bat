@echo off
setlocal

if "%1"=="clean" (
    rmdir /s /q build 2>nul
    echo 已清理 build 目录。
    exit /b 0
)

if "%1"=="debug" (
    cmake --preset ninja-debug
    if errorlevel 1 exit /b 1
    cmake --build --preset ninja-debug
    if errorlevel 1 exit /b 1
    echo 构建完成：build/ninja-debug/bin/gl-draw-desktop.exe
    exit /b 0
)

if "%1"=="" goto release
if "%1"=="release" goto release

echo 用法：build.bat [debug^|release^|clean]
exit /b 2

:release
cmake --preset ninja-release
if errorlevel 1 exit /b 1
cmake --build --preset ninja-release
if errorlevel 1 exit /b 1
echo 构建完成：build/ninja-release/bin/gl-draw-desktop.exe
exit /b 0

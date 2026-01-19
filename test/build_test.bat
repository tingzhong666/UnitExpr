@echo off
echo 构建单元表达式测试...

REM 设置CMake路径
set CMAKE_PATH="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

REM 创建构建目录
if not exist build mkdir build
cd build

REM 生成构建文件
%CMAKE_PATH% .. -G "Visual Studio 17 2022" -A x64

REM 构建项目
%CMAKE_PATH% --build . --config Release

echo.
echo 构建完成！可执行文件在 build/bin/Release/ 目录中
pause
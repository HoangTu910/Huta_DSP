@echo off
REM Biên dịch project bằng CMake
echo == Building project...
cmake --build build
echo == Running executable...
call E:\Qt\6.9.2\mingw_64\bin\qtenv2.bat
cd /d C:\Users\Hii\Desktop\Huta_DSP
REM Chạy file exe sau khi build
.\build\huta_dsp.exe
echo == Adding to testing...
call vis.bat
echo == Done.


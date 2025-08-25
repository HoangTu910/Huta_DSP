@echo off
REM Biên dịch project bằng CMake
echo == Building project...
cmake --build build
echo == Running executable...
REM Chạy file exe sau khi build
.\build\Debug\huta_dsp.exe
echo == Adding to testing...
call vis.bat
echo == Done.


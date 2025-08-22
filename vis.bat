@echo off

set SOURCE=%~dp0build\Debug
set DEST=%~dp0visualize

copy "%SOURCE%\*.txt" "%DEST%\"

echo Go to %DEST% to see the signal visualization.
pause

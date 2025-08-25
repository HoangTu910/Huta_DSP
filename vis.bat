@echo off

set DEST=%~dp0visualize

copy "*.txt" "%DEST%\"

echo Go to %DEST% to see the signal visualization.

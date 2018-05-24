echo off
set src=%1
set dest=%2
set name=%3

robocopy "%src%" "%dest%" "%name%" /s /r:0 /w:0 > nul
SET /A ERRLVL="%ERRORLEVEL & 24"
EXIT /B %ERRLVL%
@echo off
setlocal enableextensions
set root_path=%1
set sym_path=%2
for /f "tokens=*" %%a in (%sym_path%) do call :processline %%a

pause
goto :eof

:processline
set debug_id=%4
set final_path=%root_path%/%debug_id%/

md "%final_path%"
copy "%sym_path%" "%final_path%"
exit 0

:eof
@echo off
set source=%1
set destination=%2
set filter=%3

::Not sure if this is needed
::It guarantees you have a canonical path (standard form)
for %%F in (%destination%) do set destination="%%~fF"

for /r %source% %%F in (.) do if "%%~fF" neq %destination% robocopy "%%F" %destination% %filter% /copyall /r:0 /w:0 > nul
set /A ERRLVL="%ERRORLEVEL & 24"
exit /B %ERRLVL%
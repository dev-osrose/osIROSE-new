@echo off
set source=%1
set destination=%2
set filter=%3

::Not sure if this is needed
::It guarantees you have a canonical path (standard form)
for %%F in (%destination%) do set destination="%%~fF"

::for /r %source% %%F in (%filter%) do if "%%~fF" neq %destination% echo "%%F"
for /r %source% %%F in (%filter%) do xcopy /y "%%F" "%destination%" > nul
set /A ERRLVL="%ERRORLEVEL & 24"
exit /B %ERRLVL%
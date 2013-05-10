@echo off

REM - Default values
set bin=-1
set user=root
set pass=-1
set db=prose
set host=localhost
echo.
echo                  ===== OSpRose Online Database Installer ======
echo.
REM - Get MySQL Bin Path
:getbin
set /p bin="Enter the MySQL bin path: "
if %bin%==-1 goto getbin
if NOT EXIST "%bin%\mysql.exe" goto getBin
if NOT EXIST "%bin%\mysqldump.exe" goto getBin

REM - Get database username
:getuser
set /p user="Enter database username (default: root): "

REM - Get database password
:getpass
set /p pass="Enter database password: "
if %pass%==-1 goto getpass

REM - Get database name
:getdb
set /p db="Enter database name (default: prose): "

REM - Get database host
:gethost
set /p host="Enter database host (default: localhost): "

REM - Menu
:menuoptions
echo.
echo Backing up your database is HIGHLY recommended before installing a new database
echo Please select an option
echo =======================
echo 1) Backup Database
echo 2) Install Database
echo 3) Quit
:menu
set option=0
set /p option="Select an option: "
if %option%==1 goto backup
if %option%==2 goto install
if %option%==3 goto quit
goto menu

REM - Backup
:backup
echo.
echo Backing up MySQL Database %db% on host %host% to file %db%_backup.sql
%bin%\mysqldump.exe --add-drop-table -h %host% -u %user% --password=%pass% %db% > %db%_backup.sql
echo Database Backed Up
echo.
goto menuoptions

REM - Install
:install
echo.
echo Installing prose.sql to %db% on %host%
%bin%\mysql.exe -h %host% -u %user% --password=%pass% -D %db% < prose.sql
echo Database Installed
echo.
goto menuoptions

REM - Quit
:quit
echo.
echo Thanks for using the OSpRose Database Installer :-) Exiting...
pause

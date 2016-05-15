msiexec /i mysql-connector-c-6.1.6-winx64.msi /quiet /qn /norestart /log mysql-connector-c-6.1.6.log

REM 7z e mysqlpp.tar.gz && 7z x mysqlpp.tar -pC:\projects\mysqlpp -y > C:\projects\mysqlpp_install.txt
REM mkdir C:/projects/osIROSE/mysqlpp
REM Robocopy "C:/projects/mysql++-3.2.2/lib" "C:/projects/osIROSE/mysqlpp" /E /MT:2 /R:0 /DCOPY:T
REM copy "C:\projects\osIROSE\cmake\mysqlpp_CMakeFile.txt" "C:\projects\osIROSE\mysqlpp\CMakeLists.txt" /Y
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
cd ./tools/mysqlpp/lib/
perl querydef.pl
perl ssqls.pl
cd ../../..
exit 0

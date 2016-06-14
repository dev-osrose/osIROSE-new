msiexec /i mysql-connector-c-6.1.6-winx32.msi /quiet /qn /norestart /log mysql-connector-c-6.1.6.log
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
cd ./tools/mysqlpp/lib/
perl querydef.pl
perl ssqls.pl
cd ../../..
exit 0

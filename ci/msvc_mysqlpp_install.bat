cd C:/projects/
7z e mysqlpp.tar.gz && 7z x mysqlpp.tar -pC:\projects\mysqlpp -y > C:\projects\mysqlpp_install.txt
mkdir C:/projects/osIROSE/mysqlpp
Robocopy "C:/projects/mysql++-3.2.2/lib" "C:/projects/osIROSE/mysqlpp" /E /MT:2 /R:0 /DCOPY:T
copy "C:\projects\osIROSE\cmake\mysqlpp_CMakeFile.txt" "C:\projects\osIROSE\mysqlpp\CMakeFile.txt" /Y
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
cd C:/projects/osIROSE/mysqlpp/
py querydef.pl
py ssqls.pl
cd ../..
pause

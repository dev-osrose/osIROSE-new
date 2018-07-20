mkdir build
mkdir build\bin
mkdir build\bin\%CONFIGURATION%
mkdir 3rdparty\include

pip install pyparsing
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
exit 0


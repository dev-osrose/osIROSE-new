mkdir build
mkdir 3rdparty/include
REM mkdir 3rdparty\build
REM Robocopy "./3rdparty/spdlog/include" "./3rdparty/include" /E /MT:2 /R:0 /DCOPY:T
pip install pyparsing
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
exit 0


mkdir build
REM mkdir 3rdparty\build
Robocopy "./3rdparty/googletest/googlemock" "./3rdparty/protobuf/gmock" /E /MT:2 /R:0 /DCOPY:T
Robocopy "./3rdparty/googletest/googletest" "./3rdparty/protobuf/gmock/gtest" /E /MT:2 /R:0 /DCOPY:T
REM Robocopy "./3rdparty/spdlog/include" "./3rdparty/include" /E /MT:2 /R:0 /DCOPY:T
pip install pyparsing
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
exit 0


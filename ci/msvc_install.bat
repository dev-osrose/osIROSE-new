mkdir build
REM mkdir 3rdparty\build
Robocopy "./tools/googletest/googlemock" "./tools/protobuf/gmock" /E /MT:2 /R:0 /DCOPY:T
Robocopy "./tools/googletest/googletest" "./tools/protobuf/gmock/gtest" /E /MT:2 /R:0 /DCOPY:T
REM Robocopy "./tools/spdlog/include" "./3rdparty/include" /E /MT:2 /R:0 /DCOPY:T
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
exit 0


Robocopy "./tools/googletest/googlemock" "./tools/protobuf/gmock" /E /MT:2 /R:0 /DCOPY:T
Robocopy "./tools/googletest/googletest" "./tools/protobuf/gmock/gtest" /E /MT:2 /R:0 /DCOPY:T
IF %ERRORLEVEL% LSS 8 goto end
goto :eof

:end
exit 0


mkdir build
Robocopy "./tools/googletest/googlemock" "./tools/protobuf/gmock" /E /MT:2 /R:0 /DCOPY:T
Robocopy "./tools/googletest/googletest" "./tools/protobuf/gmock/gtest" /E /MT:2 /R:0 /DCOPY:T
IF %ERRORLEVEL% LSS 8 goto build
goto :eof

:build
mkdir protobuf
cd protobuf
cmake ../tools/protobuf/cmake
cmake --build . --config %CONFIGURATION% -- /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
dir
exit 0


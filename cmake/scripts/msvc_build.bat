REM @call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
SET PATH=%PROJECT_PATH%\3rdparty\bin;%MYSQL_CONNECTOR_PATH%\lib;%MYSQL_PATH%\bin;%PYTHON_PATH%\Scripts;C:\OpenSSL-v111-Win64\bin;C:\Libraries\boost_1_67_0;C:\mingw-w64\x86_64-7.2.0-posix-seh-rt_v5-rev1\mingw64\bin;

cp "%MYSQL_CONNECTOR_PATH%\lib\libmysql.dll" "%PROJECT_PATH%\build\bin\"
cp "C:\mingw-w64\x86_64-7.2.0-posix-seh-rt_v5-rev1\mingw64\opt\bin\*.dll" "%PROJECT_PATH%\build\bin\"

REM cmake -GNinja -D%CONFIGURATION%=TRUE -DCMAKE_BUILD_TYPE=%CONFIGURATION% -DCMAKE_VS_PLATFORM_TOOLSET_VERSION=15.6 ..
cmake -G"MinGW Makefiles" ..
cmake --build . --config %CONFIGURATION%

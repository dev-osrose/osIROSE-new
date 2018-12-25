@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
SET PATH=%PROJECT_PATH%\3rdparty\bin;%MYSQL_CONNECTOR_PATH%\lib;%MYSQL_PATH%\bin;%PYTHON_PATH%\Scripts;%PATH%

cp "%MYSQL_CONNECTOR_PATH%\lib\libmysql.dll" "%PROJECT_PATH%\build\bin\"

cmake -GNinja -D%CONFIGURATION%=TRUE -DCMAKE_BUILD_TYPE=%CONFIGURATION% -DCMAKE_VS_PLATFORM_TOOLSET_VERSION=15.6 ..
cmake --build . --config %CONFIGURATION%

@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
SET PATH=%PROJECT_PATH%\3rdparty\bin;%MYSQL_CONNECTOR_PATH%\lib;%MYSQL_PATH%\bin;%PYTHON_PATH%\Scripts;%PATH%

cp "%MYSQL_CONNECTOR_PATH%\lib\libmysql.dll" "%PROJECT_PATH%\build\bin\"

cmake -GNinja -D%CONFIGURATION%=TRUE -DCMAKE_BUILD_TYPE=%CONFIGURATION% ..
cmake --build . --config %CONFIGURATION%
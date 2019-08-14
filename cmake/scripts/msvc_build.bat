@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
SET PATH=%PROJECT_PATH%\3rdparty\bin;%MYSQL_CONNECTOR_PATH%\lib;%MYSQL_PATH%\bin;%PYTHON_PATH%\Scripts;C:\OpenSSL-v111-Win64\bin;C:\Libraries\boost_1_67_0;%PATH%

cp "%MYSQL_CONNECTOR_PATH%\lib\libmysql.dll" "%PROJECT_PATH%\build\bin\"

cmake -GNinja -DCMAKE_BUILD_TYPE=%CONFIGURATION% ..
cmake --build . --config %CONFIGURATION%

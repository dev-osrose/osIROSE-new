#!/bin/bash

export PATH=%PROJECT_PATH%\3rdparty\bin;%MYSQL_CONNECTOR_PATH%\lib;%MYSQL_PATH%\bin;%PYTHON_PATH%\Scripts;C:\Libraries\boost_1_67_0;%PATH%

cp "%MYSQL_CONNECTOR_PATH%\lib\libmysql.dll" "%PROJECT_PATH%\build\bin\"

cmake -G"MSYS Makefiles" ..
cmake --build .

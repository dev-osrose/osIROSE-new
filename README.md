osIROSE - Open Source International R.O.S.E Online Server
===================================================

##### Build status
| [Linux][lin-link] | [Windows][win-link] | [Coveralls][cov-link] | [Codecov][ccov-link]  | [Coverity Scan Build Status][coverity-scan-link] |
| :---------------: | :-----------------: | :-------------------: | :-------------------: | :-------------------: |
| ![lin-badge]      | ![win-badge]        | ![cov-badge]          | ![ccov-badge]         | ![coverity-scan]      |

[lin-badge]: https://travis-ci.org/RavenX8/osIROSE-new.svg?branch=trunk "Travis build status"
[lin-link]:  https://travis-ci.org/RavenX8/osIROSE-new "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/20x0eufp7djvunf3/branch/trunk?svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/RavenX8/osirose-new/branch/trunk "AppVeyor build status"
[cov-badge]: https://coveralls.io/repos/RavenX8/osIROSE-new/badge.svg?branch=trunk&service=github
[cov-link]:  https://coveralls.io/github/RavenX8/osIROSE-new?branch=trunk
[ccov-badge]: https://codecov.io/gh/RavenX8/osIROSE-new/branch/trunk/graph/badge.svg
[ccov-link]: https://codecov.io/gh/RavenX8/osIROSE-new/branch/trunk
[coverity-scan]: https://scan.coverity.com/projects/7232/badge.svg
[coverity-scan-link]: https://scan.coverity.com/projects/ravenx8-osirose-new


##### Development and Community:
[![GitHub Repository](https://img.shields.io/badge/github-RavenX8/osIROSE--new-green.svg)](https://github.com/RavenX8/osIROSE-new)
[![Waffle Board](https://img.shields.io/badge/waffle-RavenX8/osIROSE--new-6699dd.svg)](https://waffle.io/RavenX8/osIROSE-new)  
[![Community Forum](https://img.shields.io/badge/forum-http%3A%2F%2Fforum.dev--osrose.com-green.svg)](http://forum.dev-osrose.com/index.php)
[![Join the chat at https://gitter.im/RavenX8/osIROSE-new](https://badges.gitter.im/RavenX8/osIROSE-new.svg)](https://gitter.im/RavenX8/osIROSE-new?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)    

##### Issues and Status
[![GitHub issues](https://img.shields.io/github/issues/RavenX8/osIROSE-new.svg)](https://github.com/RavenX8/osIROSE-new/issues)
[![Confirmed issues](https://badge.waffle.io/RavenX8/osIROSE-new.svg?label=status:confirmed&title=Confirmed%20Issues)](http://waffle.io/RavenX8/osIROSE-new) [![In-Progress](https://badge.waffle.io/RavenX8/osIROSE-new.svg?label=status:in-progress&title=In%20Progress)](http://waffle.io/RavenX8/osIROSE-new)

##### Project Info:
![Language](https://img.shields.io/badge/language-C++-yellow.svg)
[![GitHub license](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://raw.githubusercontent.com/RavenX8/osIROSE-new/master/LICENSE.txt)   

C++ Installation - Unix
-----------------------

If you get the source from github, you need to setup the build environment:

    $ ./unix_env_setup.sh

This will install all of the tools and sources required to build the project.

"./unix_env_setup.sh" will require superuser privileges, as it calls apt-get install.

To build and install the servers, execute the following:

    $ make
    $ make install

"make install" may require superuser privileges.

To run the servers execute the following:

    $ LoginServer&
    $ CharServer&
    $ MapServer&

C++ Installation - Windows
-----------------------

##### Programs required:
[![vis-studio]][vis-studio-link]
[![cmake]][cmake-link]  
[![mysql]][mysql-link]
[![perl]][perl-link]

[vis-studio]: https://img.shields.io/badge/Visual%20Studio-Download-blue.svg "Download Visual Stuido"
[vis-studio-link]: https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx "Download Visual Stuido"
[cmake]: https://img.shields.io/badge/CMake-Download-blue.svg "Download CMake"
[cmake-link]: https://cmake.org/download/ "Download CMake"
[mysql]: https://img.shields.io/badge/MySQL%20Connector%20C%20v6.1.6%2032--bit-Download-blue.svg "Download MySQL Connector:C 32-bit"
[mysql-link]: http://dev.mysql.com/downloads/connector/c/ "Download MySQL Connector:C 32-bit"
[perl]: https://img.shields.io/badge/Perl-Download-blue.svg "Download Perl"
[perl-link]: https://www.perl.org/get.html "Download Perl"

After downloading and installing the above applications, to compile the servers run the following commands:

    git submodule update --init --recursive
    ./ci/msvc_mysqlpp_install.bat
    ./ci/msvc_install.bat
    cd build
    cmake -DBUILD_TYPE=Release -DBUILD_MYSQLPP=ON -DBUILD_PROTOBUF=ON -Dprotobuf_BUILD_TESTS=OFF ..
    cmake --build . --config Release
    
This will setup the build directories and compile. The compiled server will be in the bin folder created in the directory you ran cmake in (which should be the build folder).

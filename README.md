osIROSE - Open Source International R.O.S.E Online Server
===================================================

## Build status
| [Linux][lin-link] | [Windows][win-link] | [Coveralls][cov-link] | [Codecov][ccov-link]  |
| :---------------: | :-----------------: | :-------------------: | :-------------------: |
| ![lin-badge]      | ![win-badge]        | ![cov-badge]          | ![ccov-badge]         |

[lin-badge]: https://travis-ci.org/RavenX8/osIROSE-new.svg?branch=trunk "Travis build status"
[lin-link]:  https://travis-ci.org/RavenX8/osIROSE-new "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/20x0eufp7djvunf3/branch/trunk?svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/RavenX8/osirose-new/branch/trunk "AppVeyor build status"
[cov-badge]: https://coveralls.io/repos/RavenX8/osIROSE-new/badge.svg?branch=trunk&service=github
[cov-link]:  https://coveralls.io/github/RavenX8/osIROSE-new?branch=trunk
[ccov-badge]: https://codecov.io/gh/RavenX8/osIROSE-new/branch/trunk/graph/badge.svg
[ccov-link]: https://codecov.io/gh/RavenX8/osIROSE-new/branch/trunk
[site]: https://img.shields.io/badge/Official%20website-http%3A%2F%2Fforum.dev--osrose.com-green.svg "Official Site"

## Links
[![site]](http://forum.dev-osrose.com/index.php)

[![Join the chat at https://gitter.im/RavenX8/osIROSE-new](https://badges.gitter.im/RavenX8/osIROSE-new.svg)](https://gitter.im/RavenX8/osIROSE-new?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Confirmed issues](https://badge.waffle.io/RavenX8/osIROSE-new.svg?label=status:confirmed&title=Confirmed%20Issues)](http://waffle.io/RavenX8/osIROSE-new) [![In-Progress](https://badge.waffle.io/RavenX8/osIROSE-new.svg?label=status:in-progress&title=In%20Progress)](http://waffle.io/RavenX8/osIROSE-new)

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

C++ Installation - Windows (OUTDATED)
-----------------------

To compile the servers run the following commands:

    ./ci/msvc_install.bat
    cd build
    cmake -DBUILD_PROTOBUF=ON ..
    cmake --build .
    
This will setup the build directories and compile. The compiled server will be in the bin folder created in the directory you ran cmake in.

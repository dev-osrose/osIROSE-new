osIROSE - Open Source International R.O.S.E Online Server
===================================================

Linux [![Build Status](https://travis-ci.org/RavenX8/osIROSE-new.svg)](https://travis-ci.org/RavenX8/osIROSE-new)
Windows [![Build status](https://ci.appveyor.com/api/projects/status/20x0eufp7djvunf3?svg=true)](https://ci.appveyor.com/project/RavenX8/osirose-new)
Unit Tests [![Coverage Status](https://coveralls.io/repos/RavenX8/osIROSE-new/badge.svg?branch=trunk&service=github)](https://coveralls.io/github/RavenX8/osIROSE-new?branch=trunk)

[Official website](http://forum.dev-osrose.com/index.php)

C++ Installation - Unix
-----------------------

If you get the source from github, you need to setup the build environment:

    $ ./setupEnv.sh

This will install all of the tools and sources required to build the project.

"./setupEnv.sh" may require superuser privileges, as it calls apt-get install.

To build and install the servers, execute the following:

    $ make
    $ make install

"make install" may require superuser privileges.

To run the servers execute the following:

    $ LoginServer
    $ CharServer
    $ WorldServer


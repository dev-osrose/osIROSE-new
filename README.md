# osIROSE - Open Source International R.O.S.E Online Server

## Build status
[![lin-badge]][lin-link] [![win-badge]][win-link]
[![cov-badge]][cov-link] [![ccov-badge]][ccov-link]
[![coverity-scan]][coverity-scan-link]

## Development and Community
[![GitHub Repository](https://img.shields.io/badge/github-dev--osrose/osIROSE--new-green.svg)](https://github.com/dev-osrose/osIROSE-new)
[![Community Forum](https://img.shields.io/badge/forum-http%3A%2F%2Fforum.dev--osrose.com-green.svg)](http://forum.dev-osrose.com/index.php)
[![Join the chat in Discord](https://img.shields.io/discord/454345032846016515.svg)](https://discord.gg/u7UE5fW)

## Issues and Status
[![GitHub issues](https://img.shields.io/github/issues/dev-osrose/osIROSE-new.svg)](https://github.com/dev-osrose/osIROSE-new/issues)

## Project Info
![Language](https://img.shields.io/badge/language-C++-yellow.svg)
[![GitHub license](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://raw.githubusercontent.com/dev-osrose/osIROSE-new/master/LICENSE.txt)

## Software Services Used

Thanks to the following services that make our development easier:

- Crash reporting analysis provided by [![backtrace-badge]][backtrace-link]

## C++ Installation - Unix

### Unix software required
![gpp]
![libmysql]
[![cmake]][cmake-link]
[![python]][python-link]

If you get the source from github, you need to setup the build environment:

```cmd
    git submodule update --init --recursive
    pip install --user pyparsing
    mkdir build
    cd build
    cmake ..
```

Note, you may need to install the following packages depending on your OS:

```cmd
    unzip
    autoconf
    libtool
    libssl-dev
    libncurses5-dev
    libreadline-dev
```

To build the servers, execute the following:

```cmd
    make
```

To install the servers to the system (not recommended), execute the following commands:

```cmd
    make install
```

"make install" may require superuser privileges.

If you installed the servers, to run the servers execute the following:

```cmd
    LoginServer&
    CharServer&
    MapServer&
```

If you opted to keep them in place, the server binaries are located in the bin folder inside the build directory. So from where you ran the command make, you will want to run the following:

```cmd
    ./bin/LoginServer&
    ./bin/CharServer&
    ./bin/MapServer&
```

After running for the first time, the servers will create a config file in the current directory that you need to edit.

## C++ Installation - Windows

### Windows 64bit software required
[![vis-studio]][vis-studio-link]
[![cmake]][cmake-link]
[![mysql]][mysql-link]
[![python]][python-link]
[![openssl]][openssl-link]

After downloading and installing the above applications, to compile the servers run the following commands:

```cmd
    git submodule update --init --recursive
    pip install pyparsing
    mkdir build
    cd build
    cmake ..
    cmake --build . --config Release
```

This will setup the build directories and compile. The compiled server will be in the bin folder created in the directory you ran cmake in (which should be the build folder).

After running for the first time, the servers will create a config file in the current directory that can be modified.

## Server Configuration

The servers can be configured using a `server.json` file. This file is automatically generated if any
server is started and no `server.json` file exists. It's recommended to generate this file before
continuing with the project setup. Simply start any of the servers to generate the file.

## Client Setup

The server is designed to work with the assets from the `ROSE_112_112NA` client. Using different
assets may cause issues due to incompatibility. The client is available to download
[here](https://mega.nz/#F!8AkG3TzJ!6eoMc65tHmF_d_6nAvf8ZA?UQc3zZKb).

Run the installer and install the game to a desired location. While this project uses the assets
from that client it does not use the binaries. For full compatibility you will need to download
the latest client binaries from the project's [Releases Page](https://github.com/dev-osrose/osIROSE-new/releases)

> 12/5/2019 - Until a new client release is up on github you'll need to use the client from discord: [SHA256 Client](https://cdn.discordapp.com/attachments/454345032846016519/625869971284557824/sha256_client_release_fixed.zip)

After downloading the client files, copy them to the `ROSE_112_112NA` installation directory (default:
`C:\Program Files (x86)\TriggerSoft\Rose Online`) and overwrite the existing files.

 Connecting to the server requires using a special command from the command line, like so:

`trose.exe @TRIGGER_SOFT@ _server 127.0.0.1` (replace the IP if not running locally

For convenience, this command can be saved to a file with the extension `.cmd` in the same folder
as `trose.exe`. Then, the game can be launched by double clicking this file.

## Database Setup

The server requires a [MySQL](https://mysql.com/) (or [MariaDB](https://mariadb.com/)) database server.

To setup the database, first create a new database (e.g. `osirose`). You will then need to execute the
`Database/osirose.sql` script to generate the database schema and the `Database/item_db.sql` to
populate the item database. This can be done using a database tool or from the command line like so:
```cmd
  mysql -u<user> -p<password> <database_name> < Database/osirose.sql
  mysql -u<user> -p<password> <database_name> < Database/item_db.sql
```

**NOTE**: The item table was generated using client v112_112na. If you are using a different client,
please generate your own table to match the information you need.

After setting up the database, two special users need to be created for the login and character
servers. They can be created by executing the following SQL commands:

```sql
  call create_account("login", "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08");
  call create_account("char", "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08");
  update accounts set account_type="system" where username="login";
  update accounts set account_type="system" where username="char";
```

The value `9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08` is an SHA256 hash of the default server passwords created
in the default `server.json` file. It should match the following sections:

```json
    ...
    "charServer":  {
        ...
        "loginUser":         "login",
        "loginPassword":     "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        ...
    },
    "mapServer":   {
        ...
        "charUser":     "char",
        "charPassword": "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        ...
    },
    ...
```

To use your own password, generate an [SHA256 hash](https://emn178.github.io/online-tools/sha256.html) of your password
and update the accounts database and the `server.json` file.

Finally, we need to create an account to join the game with. Run the `create_account` procedure
again to create a user account, for example:

```sql
  call create_account("admin", "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918") # Password is admin
```

Alternatively, automatic account creation can be enabled in the server config. This will automatically
create an account if a user attempts to login to an account that exist. This is recommended as it is
much more convenient than having to create password hashes and running sql commands. Simply, try to
log in, fail, then try to login again with the same credentials!

To enable it modify `server.json` to enable `createAccountOnFail`:
```json
  "loginServer": {
      "createAccountOnFail": true,
      ...
  },
```

## Contributing

If you want to contribute, please read the [code of conduct](CODE_OF_CONDUCT.md). You can find some documentation about the source code [here](Documentation.md). Don't hesitate to contribute!
 
[lin-badge]: https://travis-ci.com/dev-osrose/osIROSE-new.svg?branch=trunk "Linux build status"
[lin-link]:  https://travis-ci.com/dev-osrose/osIROSE-new "Linux build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/20x0eufp7djvunf3/branch/trunk?svg=true "Windows build status"
[win-link]:  https://ci.appveyor.com/project/RavenX8/osirose-new/branch/trunk "Windows build status"
[cov-badge]: https://coveralls.io/repos/github/dev-osrose/osIROSE-new/badge.svg?branch=trunk&service=github
[cov-link]:  https://coveralls.io/github/dev-osrose/osIROSE-new?branch=trunk
[ccov-badge]: https://codecov.io/gh/dev-osrose/osIROSE-new/branch/trunk/graph/badge.svg
[ccov-link]: https://codecov.io/gh/dev-osrose/osIROSE-new/branch/trunk
[coverity-scan]: https://scan.coverity.com/projects/7232/badge.svg
[coverity-scan-link]: https://scan.coverity.com/projects/dev-osrose-osirose-new
[backtrace-badge]: https://img.shields.io/badge/Crash%20Report%20Analysis-backtrace.io-informational
[backtrace-link]: https://backtrace.io
[gpp]: https://img.shields.io/badge/g++-v7%20or%20higher-blue.svg "G++ 7 or higher"
[libmysql]: https://img.shields.io/badge/libmysql-v5.7%20or%20higher-blue.svg "libmysql 5.7 or higher"
[vis-studio]: https://img.shields.io/badge/Visual%20Studio-Download-blue.svg "Download Visual Stuido"
[vis-studio-link]: https://visualstudio.microsoft.com/downloads/ "Download Visual Stuido"
[cmake]: https://img.shields.io/badge/CMake-Download-blue.svg "Download CMake"
[cmake-link]: https://cmake.org/download/ "Download CMake"
[mysql]: https://img.shields.io/badge/MySQL%20Connector%20C%20x64--bit-Download-blue.svg "Download MySQL Connector:C 64-bit"
[mysql-link]: http://dev.mysql.com/downloads/connector/c/ "Download MySQL Connector:C 64-bit"
[python]: https://img.shields.io/badge/Python%202.7-Download-blue.svg "Download Python 2.7"
[python-link]: https://www.python.org/downloads/release/python-2715/ "Download Python 2.7"
[openssl]: https://img.shields.io/badge/OpenSSL%201.0.2t%20x64-Download-blue.svg "Download OpenSSL 1.0.2t x64"
[openssl-link]: https://slproweb.com/products/Win32OpenSSL.html "Download OpenSSL x64"

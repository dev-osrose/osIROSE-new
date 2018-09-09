#Documentation

As with most rose servers, the project has 3 different executables: login, char and map. This is done to allow for greater flexibility and better resource management.
We are also working on a 4th executable: the node server. This executable will be responsible for rerouting all the packets from the client to the correct map server (assuming you have multiple). If you only have one map server, this executable will not be needed.

The project is separated in 10 parts:
* [Database](Database/database.md) That's the database schema and basic item information
* [Scripts](Scripts/scripts.md) That's the lua code for gameplay scripting
* [Automated testing(bot)](src/bot/bot.md) That's the code for a pseudo client that does integration testing against the server
* [Core](src/core/core.md) The core code that contains all of the shared code (database, factories, threading, networking and so on)
* [RoseCommon](src/rosecommon/rosecommon.md) All the code common to all three servers that is rose-specific
* [LoginServer](src/login/login.md) The login server is coded here
* [CharServer](src/char/char.md) The character server is coded here
* [WorldServer](src/map/map.md) The world server
* [NodeServer](src/node/node.md) The node server
* [Tests](src/tests/tests.md) All of the unit tests for the codebase

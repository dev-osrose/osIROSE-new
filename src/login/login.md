# Login server
The login server implementation. It contains 3 classes:
* CLoginServer: the login server implementation. Manages the clients
* CLoginIsc: the communication code between servers. Manages the internal state of keeping up to date with the other servers (for up/down times and more)
* CLoginClient: the client implementation. Most of the logic is implemented in this class.

The `main.cpp` file contains the code to remove stale [sessions](../../Database/database.md#table-sessions).

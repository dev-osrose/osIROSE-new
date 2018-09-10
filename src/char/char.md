# Char server
The char server is very similar to the [login server](../login/login.md) in terms of architecture.
The only difference is the packets used and the logic behind it.
There is also a lot more database access than the login server.
All of the logic is contained within the CCharClient implementation.

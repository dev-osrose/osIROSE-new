# RoseCommon library
The rosecommon library contains all of the common rose-related code for all the projects (bot, login, char, map and node).
The `packets/` folder contains all of the flatbuffer implementation of the packets (in-progress).
The library includes:
* components: All of the components used in the [ECS (entity component system)](ecs.md)
* packets: All of the implemented packets using the custom packet implementation (CRosePacket)
* CRoseClient: a client associated with a socket
* CRoseCrypt: the cryptography engine used to encrypt and decrypt the packets
* CRoseISC: The communication channel between servers
* CRosePacket: the custom packet implementation.
* CRoseServer: a server implementation that holds onto CRoseClients
* CRoseSocket: a socket implementation on top of the core networking socket. Each CRoseClient and CRoseServer use that implementation
* dataconsts: random data constants that shouldn't be here
* EntityApi: the lua API implementation for monsters and npcs (any kind of entity really, so it could be a fake player or anything)
* ePacketType: the enum with all of the packet ids
* isccommon: the isc packets for inter server communication
* Item: the item implementation
* ItemApi: the lua API implementation for items
* ItemDb: the item database cache. It loads the data from the database on startup and provides a nice api to fetch the necessary details
* LuaApi: the base class for the lua API
* PacketFactory: the packet factory (this thing is a big mess, but basically you can use makePacket<ePacketType>(args...) to create a packet)
* PartyBase: the base class for a party (required to avoid cyclic dependencies in the components. Also required because the component is shared between all entities that are in the same party)

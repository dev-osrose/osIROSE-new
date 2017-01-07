#pragma once

class CMapClient;

struct SocketConnector {
    SocketConnector(CMapClient *client) : client_(client) {}

    // FIXME : use an atomic type instead?
    CMapClient *client_;
};


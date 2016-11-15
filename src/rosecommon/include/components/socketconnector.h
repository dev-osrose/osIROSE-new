#pragma once

class CMapClient;

struct SocketConnector {
    SocketConnector(CMapClient *client) : client_(client) {}

    CMapClient *client_;
};


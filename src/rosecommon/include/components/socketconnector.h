#pragma once

#include <memory>

class CMapClient;

struct SocketConnector {
    SocketConnector(CMapClient *client) : client_(client) {}

    CMapClient *client_;
};


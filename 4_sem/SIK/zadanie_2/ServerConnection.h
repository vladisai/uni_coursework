#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <netdb.h>
#include <utility>
#include <vector>

#include "ClientMessage.h"
#include "ServerMessage.h"

class ServerConnection {
public:
    using ClientAddress = sockaddr_in6;
    using ClientAddressMessagePair = std::pair<ClientAddress, ClientMessage>;

    ServerConnection(int port) : port(port), sock(-1) {}

    bool connect();

    ClientAddressMessagePair receive();

    std::pair<ClientAddress, RawData> receiveData();

    bool send(ClientAddress, const ServerMessage &);

    bool send(ClientAddress, const RawData &);

    bool disconnect();

private:

    int port;
    int sock;

};

#endif // SERVERCONNECTION_H

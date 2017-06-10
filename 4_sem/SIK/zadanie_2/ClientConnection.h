#ifndef NETACKA_SIK_CLIENTCONNECTION_H
#define NETACKA_SIK_CLIENTCONNECTION_H

#include "Serializer.h"
#include "ClientMessage.h"
#include "ServerMessage.h"
#include "Serializer.h"

#include <netinet/in.h>

class ClientConnection {
public:
    ClientConnection(const std::string& host, std::string& port);

    bool connect();

    bool send(const RawData &data);

    bool send(const ClientMessage& message);

    RawData receiveData();

    ServerMessage receiveMessage();

    bool disconnect();

private:
    sockaddr *my_address;
    socklen_t my_address_len;
    std::string host;
    std::string port;
    int sock;
};


#endif //NETACKA_SIK_CLIENTCONNECTION_H

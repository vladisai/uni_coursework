#include "ServerConnection.h"

#include <string.h>
#include <zconf.h>
#include <thread>

#include "fail.h"
#include "ServerConfig.h"

bool ServerConnection::connect() {
    sockaddr_in6 server_address;
    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock < 0) {
        failSysErrorExit("socket failed");
    }
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin6_family = AF_INET6;
    server_address.sin6_addr = in6addr_any;
    server_address.sin6_port = htons((uint16_t)port);

    if (bind(sock, (struct sockaddr *)&server_address,
             (socklen_t)sizeof(server_address)) < 0) {
        failSysErrorExit("bind failed");
    }
    return true;
}

ServerConnection::ClientAddressMessagePair ServerConnection::receive() {
    std::pair<ClientAddress, RawData> data = receiveData();
    return {data.first, ClientMessage::deserialize(data.second)};
}

std::pair<ServerConnection::ClientAddress, RawData> ServerConnection::receiveData() {
    if (sock == -1) {
        failSysError("receive() called before connect()"); 
    }
    sockaddr_in6 client_address;
    socklen_t rcva_len = (socklen_t)sizeof(client_address);
    int flags = 0; // we do not request anything special
    char buffer[ServerConfig::bufferSize];
    ssize_t len = recvfrom(sock, buffer, sizeof(buffer), flags,
                       (sockaddr *)&client_address, &rcva_len);
    return {client_address, RawData(buffer, buffer + len)};
}

bool ServerConnection::disconnect() {
    if (close(sock) == -1) {
        failSysErrorExit("couldn't close the socket");
    }
    sock = -1;
    return true;
}

bool ServerConnection::send(ClientAddress clientAddress, const RawData &data)
{
    int flags = 0;
    socklen_t snda_len = (socklen_t)sizeof(clientAddress);
    sendto(sock, &data[0], data.size(), flags,
                            (struct sockaddr*)&clientAddress, snda_len);
    return true;
}

bool ServerConnection::send(ClientAddress client_address, const ServerMessage& message)
{
    return send(client_address, message.serialize());
}

#include "ServerConnection.h"

#include <string.h>
#include <zconf.h>
#include <thread>

#include "Fail.h"
#include "ServerConfig.h"

bool ServerConnection::connect() {
    addrinfo addr_hints;
    addrinfo *addr_result;

    memset(&addr_hints, 0, sizeof(addrinfo));
    addr_hints.ai_family = AF_INET6;
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, std::to_string(port).c_str(), &addr_hints, &addr_result)) {
        failSysErrorExit("ServerConnection: getaddrinfo");
    }

    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0) {
        failSysErrorExit("ServerConnection: socket failed");
    }

    if (bind(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0) {
        failSysErrorExit("ServerConnection: bind failed");
    }

    timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0; // timeout for socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        failSysErrorExit("ClientConnection: couldn't set socket timeout");
    }

    return true;
}

ServerConnection::ClientAddressMessagePair ServerConnection::receive() {
    std::pair<ClientAddress, RawData> data = receiveData();
    return {data.first, ClientMessage::deserialize(data.second)};
}

std::pair<ServerConnection::ClientAddress, RawData> ServerConnection::receiveData() {
    if (sock == -1) {
        failSysErrorExit("ServerConnection: receive() called before connect()");
    }
    sockaddr_in6 client_address;
    socklen_t rcva_len = (socklen_t)sizeof(client_address);
    int flags = 0; // we do not request anything special
    char buffer[ServerConfig::bufferSize];
    ssize_t len = recvfrom(sock, buffer, sizeof(buffer), flags,
                       (sockaddr *)&client_address, &rcva_len);
    if (len < 0) {
        failSysError("ServerConnection: recvfrom failed or timed out");
        return {};
    }
    return {client_address, RawData(buffer, buffer + len)};
}

bool ServerConnection::disconnect() {
    if (close(sock) == -1) {
        failSysErrorExit("ServerConnection: couldn't close the socket");
    }
    sock = -1;
    return true;
}

bool ServerConnection::send(ClientAddress clientAddress, const RawData &data)
{
    std::cerr << "ServerConnection: sending" << std::endl;
    int flags = 0;
    socklen_t snda_len = (socklen_t)sizeof(clientAddress);
    ssize_t len = sendto(sock, &data[0], data.size(), flags,
                            (struct sockaddr*)&clientAddress, snda_len);
    if (len < 0) {
        failSysError("ServerConnection: sendto failed");
        return false;
    }
    return true;
}

bool ServerConnection::send(ClientAddress client_address, const ServerMessage& message)
{
    return send(client_address, message.serialize());
}

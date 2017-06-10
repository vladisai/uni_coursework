#include "ClientConnection.h"
#include "Fail.h"

#include <netdb.h>
#include <unistd.h>
#include <iostream>

const int BUFFER_SIZE = 512;
const int CONNECTION_TIMEOUT = 250;

ClientConnection::ClientConnection(const std::string &host, std::string &port) : host(host), port(port) {
}

bool ClientConnection::connect() {
    addrinfo addr_hints;
    addrinfo *addr_result;

    memset(&addr_hints, 0, sizeof(addrinfo));
    addr_hints.ai_family = AF_UNSPEC;
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(host.c_str(), port.c_str(), &addr_hints, &addr_result) != 0) {
        failSysErrorExit("getaddrinfo");
    }

    my_address = addr_result->ai_addr;
    my_address_len = addr_result->ai_addrlen;

    sock = socket(addr_result->ai_addr->sa_family,
                  addr_result->ai_socktype,
                  addr_result->ai_protocol);

    if (sock < 0) {
        failSysErrorExit("ClientConnection: couldn't open a socket");
    }

    timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0; // timeout for socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        failSysErrorExit("ClientConnection: couldn't set socket timeout");
    }

    return true;
}

bool ClientConnection::send(const RawData &message) {
    int flags = 0;
    ssize_t snd_len = sendto(sock, &message[0], message.size(), flags,
                            my_address, my_address_len);
    if (snd_len <= 0) {
        failSysError("ClientConnection: send failed");
        return false;
    }
    return true;
}

bool ClientConnection::send(const ClientMessage &message) {
    RawData messageData = message.serialize();
    return send(messageData);
}

RawData ClientConnection::receiveData() {
    int flags = 0;
    char buffer[BUFFER_SIZE];
    size_t len = (size_t) sizeof(buffer);
    ssize_t rcv_len = recvfrom(sock, buffer, len, flags, 0, 0);
    if (rcv_len < 0) {
        failSysError("ClientConnection: recvfrom failed or timed out");
        return {};
    }
    return RawData(buffer, buffer + rcv_len);
}

ServerMessage ClientConnection::receiveMessage() {
    return ServerMessage::deserialize(receiveData());
}

bool ClientConnection::disconnect() {
    if (close(sock) == -1) {
        failSysErrorExit("couldn't close the socket");
    }
    return true;
}

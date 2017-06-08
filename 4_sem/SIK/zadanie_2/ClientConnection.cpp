#include "ClientConnection.h"
#include "fail.h"

#include <netdb.h>
#include <unistd.h>
#include <iostream>

const int BUFFER_SIZE = 1000;
const int CONNECTION_TIMEOUT = 250;

ClientConnection::ClientConnection(const std::string &host, int port) : host(host), port(port) {
}

bool ClientConnection::connect() {
    addrinfo addr_hints;
    addrinfo *addr_result;

    memset(&addr_hints, 0, sizeof(addrinfo));
    addr_hints.ai_family = AF_UNSPEC;
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &addr_hints, &addr_result) != 0) {
        failSysErrorExit("getaddrinfo");
    }

    my_address = addr_result->ai_addr;
    my_address_len = addr_result->ai_addrlen;

    freeaddrinfo(addr_result);

    sock = socket(my_address->sa_family, SOCK_DGRAM, 0);

    if (sock < 0) {
        failSysErrorExit("couldn't open a socket");
    }

    return true;
}

bool ClientConnection::send(const RawData &message) {
    int flags = 0;
    ssize_t snd_len = sendto(sock, &message[0], message.size(), flags,
                            my_address, my_address_len);
    if (snd_len < 0) {
        failSysError("sendto failed");
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
        failSysError("rcv_len < 0");
    }
    return RawData(buffer, buffer + rcv_len);
}

ServerMessage ClientConnection::receiveMessage() {
    // Todo
}

bool ClientConnection::disconnect() {
    if (close(sock) == -1) {
        failSysErrorExit("couldn't close the socket");
    }
    return true;
}

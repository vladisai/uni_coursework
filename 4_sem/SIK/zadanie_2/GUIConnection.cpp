#include "GUIConnection.h"
#include "Fail.h"

#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <future>
#include <netinet/tcp.h>

const std::string GUIConnection::LEFT_KEY_DOWN_MSG = "LEFT_KEY_DOWN";
const std::string GUIConnection::LEFT_KEY_UP_MSG = "LEFT_KEY_UP";
const std::string GUIConnection::RIGHT_KEY_DOWN_MSG = "RIGHT_KEY_DOWN";
const std::string GUIConnection::RIGHT_KEY_UP_MSG = "RIGHT_KEY_UP";

GUIConnection::GUIConnection(const std::string &host, const std::string &port) :
        host(host), port(port), currentDirection(Position::TurnDirection::Straight) {
}

bool GUIConnection::connect() {
    addrinfo addr_hints;
    addrinfo *addr_result;

    memset(&addr_hints, 0, sizeof(addrinfo));
    addr_hints.ai_family = AF_UNSPEC;
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host.c_str(), port.c_str(), &addr_hints, &addr_result)) {
        failSysErrorExit("GUIConnection: getaddrinfo");
    }

    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0) {
        failSysErrorExit("GUIConnection: sock");
    }

    if (::connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0) {
        failSysErrorExit("GUIConnection: connect");
    }
    timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0; // timeout for socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        failSysErrorExit("GUIConnection:  couldn't set socket timeout");
    }
    int flag = 1;
    int result = setsockopt(sock,            /* socket affected */
                        IPPROTO_TCP,     /* set option at TCP level */
                        TCP_NODELAY,     /* name of option */
                        (char *) &flag,  /* the cast is historical cruft */
                        sizeof(int));    /* length of option value */
    if (result < 0) {
        failSysErrorExit("GUIConnection: couldn't turn off Nagel's algorithm");
    }

    freeaddrinfo(addr_result);
    messageLoop = std::thread(receiveLoop, sock, this);
    return true;
}

Position::TurnDirection GUIConnection::getCurrentDirection() {
    return currentDirection;
}

bool GUIConnection::close() {
    shouldStop = true;
    messageLoop.join();
    if (::close(sock) == -1) {
        failSysErrorExit("GUIConnection: couldn't close the socket");
    }
    return true;
}

void GUIConnection::send(const std::string &s) {
    ssize_t ret = write(sock, s.c_str(), s.size());
    if (ret < 0) {
        failSysError("GUIConnection: send");
    }
}

void GUIConnection::receiveLoop(int sock, GUIConnection *connection) {
    while (!connection->shouldStop) {
        char buf[100];
        int status = read(sock, buf, 100);
        std::string message(buf);
        if (message.find("\n") != -1) {
            message.erase(message.find("\n"));
        }
        if (status < 0) {
            failSysError("GUIConnection: read failed or timed out");
            continue;
        }
        if (message == LEFT_KEY_DOWN_MSG) {
            connection->setCurrentDirection(Position::TurnDirection::Left);
        } else if (message == RIGHT_KEY_DOWN_MSG) {
            connection->setCurrentDirection(Position::TurnDirection::Right);
        } else if (message == RIGHT_KEY_UP_MSG || message == LEFT_KEY_UP_MSG){
            connection->setCurrentDirection(Position::TurnDirection::Straight);
        } else {
            std::cerr << "GUIConnection: unknown gui messsage <" << buf << ">" << std::endl;
        }
    }
}

void GUIConnection::setCurrentDirection(Position::TurnDirection direction) {
    std::lock_guard<std::mutex> lock(m);
    currentDirection = direction;
}

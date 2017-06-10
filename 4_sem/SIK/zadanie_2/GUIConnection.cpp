#include "GUIConnection.h"
#include "Fail.h"

#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <future>

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
        failSysErrorExit("getaddrinfo");
    }

    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0) {
        failSysErrorExit("sock");
    }

    if (::connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0) {
        failSysError("connect");
    }
    freeaddrinfo(addr_result);
    std::thread(receiveLoop, sock, this).detach();
}

Position::TurnDirection GUIConnection::getCurrentDirection() {
    return currentDirection;
}

bool GUIConnection::close() {
}

void GUIConnection::send(const std::string &s) {
    ssize_t ret = write(sock, s.c_str(), s.size());
    if (ret < 0) {
        failSysError("send");
    }
}

void GUIConnection::receiveLoop(int sock, GUIConnection *connection) {
    while (true) {
        char buf[100];
        int status = read(sock, buf, 100);
        std::string message(buf);
        if (message.find("\n") != -1) {
            message.erase(message.find("\n"));
        }
        if (status < 0) {
            failSysError("recv");
        }
        if (message == LEFT_KEY_DOWN_MSG) {
            connection->setCurrentDirection(Position::TurnDirection::Left);
        } else if (message == RIGHT_KEY_DOWN_MSG) {
            connection->setCurrentDirection(Position::TurnDirection::Right);
        } else if (message == RIGHT_KEY_UP_MSG || message == LEFT_KEY_UP_MSG){
            connection->setCurrentDirection(Position::TurnDirection::Straight);
        } else {
            std::cerr << "unknown gui messsage <" << buf << ">" << std::endl;
        }
    }
}

void GUIConnection::setCurrentDirection(Position::TurnDirection direction) {
    std::lock_guard<std::mutex> lock(m);
    currentDirection = direction;
}

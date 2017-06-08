#ifndef NETACKA_SIK_CLIENT_H
#define NETACKA_SIK_CLIENT_H

#include <memory>

#include "ServerConnection.h"

class Client {
public:
    using SharedPtr = std::shared_ptr<Client>;

    Client(ServerConnection::ClientAddress address,
           uint64_t sessionId,
           std::string name,
           uint32_t nextExpectedEventNo) :
            address(address),
            sessionId(sessionId),
            name(name),
            nextExpectedEventNo(nextExpectedEventNo) {}

    bool operator==(const Client &other) const;

    ServerConnection::ClientAddress getAddress();

    std::string getName();

private:
    time_t lastActiveTime;
    ServerConnection::ClientAddress address;
    uint64_t sessionId;
    std::string name;
    uint32_t nextExpectedEventNo;
    bool isActive;
    bool isPlaying;
};


#endif //NETACKA_SIK_CLIENT_H
